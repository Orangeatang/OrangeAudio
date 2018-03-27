
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEVoice.h"
#include "OAESource.h"
#include "OAESourceManager.h"


//////////////////////////////////////////////////////////////////////////
/// Globals
//////////////////////////////////////////////////////////////////////////

namespace
{
    static OAUInt32 g_streamingBufferSize   = 65536;
    static OAUInt8  g_streamingBufferCount  = 2;
}


//////////////////////////////////////////////////////////////////////////
/// COAESoundInstance
//////////////////////////////////////////////////////////////////////////

COAEVoice::COAEVoice( const OAVoiceId& aVoiceId, const OASourceId& aSourceId ) :
    m_id( aVoiceId ),
    m_audioSourceId( aSourceId ),
    m_state( EVoiceState::EVoiceState_Invalid ),
    m_voice( nullptr ),
	m_totalDataSize( 0 ),
	m_totalBytesRead( 0 ),
	m_sourceType( ESourceType::ESourceType_Memory )
{
}

//////////////////////////////////////////////////////////////////////////

COAEVoice::~COAEVoice()
{
	Uninitialize();
}

//////////////////////////////////////////////////////////////////////////

bool COAEVoice::Initialize( IXAudio2* anXaudio2Interface, COAESourceManager* aSourceManager )
{
    if( anXaudio2Interface == nullptr )
    {
        return false;
    }

    // make sure the source is valid
    OASourcePtr audioSource = aSourceManager->GetSource( m_audioSourceId );
    if( audioSource == nullptr || !audioSource->IsValid() )
    {
        return false;
    }

    // create the voice
    HRESULT result = anXaudio2Interface->CreateSourceVoice( &m_voice, (WAVEFORMATEX*)audioSource->GetWaveFormat(), 0, 1.0f, this );
    if( result != S_OK )
    {
        return false;
    }

    // initialize the xaudio2 buffer(s)
	m_sourceType	= audioSource->GetType();
	m_totalDataSize = audioSource->GetDataSize();
    if( !InitializeAudioBuffers() )
    {
        return false;
    }

    m_state = EVoiceState::EVoiceState_Ready;
    return true;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::Uninitialize()
{
	// clear up the audio buffer
	for (auto buffer : m_xaudioBuffers)
	{
		delete buffer;
		buffer = nullptr;
	}
	m_xaudioBuffers.clear();

	// reset the state
	m_state = EVoiceState::EVoiceState_Invalid;
}

//////////////////////////////////////////////////////////////////////////

bool COAEVoice::Play()
{
    if( m_state != EVoiceState::EVoiceState_Ready )
    {
        return false;
    }

    HRESULT result = m_voice->Start();
    if( result != S_OK )
    {
        return false;
    }

    m_state = EVoiceState::EVoiceState_Playing;
    return true;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::Stop()
{
    if( m_state != EVoiceState::EVoiceState_Playing )
    {
        return;
    }

	// stop the voice
    m_voice->Stop();
    m_state = EVoiceState::EVoiceState_Stopped;

	// clear the audio data
	for( XAUDIO2_BUFFER* buffer : m_xaudioBuffers )
	{
		buffer->pContext = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::Update( COAESourceManager* aSourceManager )
{
	if( m_state != EVoiceState::EVoiceState_Playing )
	{
		return;
	}

    OASourcePtr audioSource = aSourceManager->GetSource( m_audioSourceId );
    if( audioSource == nullptr )
    {
        Stop();
        return;
    }

	PopulateNextAudioBuffer( audioSource );
}

//////////////////////////////////////////////////////////////////////////

const OAVoiceId& COAEVoice::GetId() const
{
    return m_id;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnVoiceProcessingPassStart( UINT32 aSamplesRequired )
{
	// submit the next available audio buffer
	XAUDIO2_BUFFER* audioBuffer = GetNextFullBuffer();
	if( audioBuffer )
	{
		// signal that the buffer is in use by setting the context
		audioBuffer->pContext = audioBuffer;
		m_voice->SubmitSourceBuffer( audioBuffer );
	}
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnVoiceProcessingPassEnd()
{
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnStreamEnd()
{
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnBufferStart( void* aBufferContext )
{
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnBufferEnd( void* aBufferContext )
{
	// extract the context
	XAUDIO2_BUFFER* audioBuffer = (XAUDIO2_BUFFER*)aBufferContext;
	if( !audioBuffer )
	{
		return;
	}

	// signal that we are no longer using the buffer
	audioBuffer->AudioBytes = 0;
	audioBuffer->pContext	= nullptr;

	if( m_totalDataSize == m_totalBytesRead )
	{
		Stop();
	}
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnLoopEnd( void* aBufferContext )
{
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnVoiceError( void* aBufferContext, HRESULT anError )
{
}

//////////////////////////////////////////////////////////////////////////

bool COAEVoice::InitializeAudioBuffers()
{
	// initialize the appropriate number of buffers based on whether the source is streaming
	OAUInt32 bufferCount	= 0;
	OAUInt32 bufferSize		= 0;
	switch( m_sourceType )
	{
	case ESourceType::ESourceType_Memory:
		bufferCount = 1;
		bufferSize  = m_totalDataSize;
		break;

	case ESourceType::ESourceType_Procedural:
	case ESourceType::ESourceType_Streaming:
		bufferCount = g_streamingBufferCount;
		bufferSize  = g_streamingBufferSize;
		break;

	default:
		return false;
	}

	for( OAUInt8 i = 0; i < bufferCount; ++i )
	{
		XAUDIO2_BUFFER* newBuffer	= new XAUDIO2_BUFFER( {0} );
		newBuffer->pAudioData		= new BYTE[bufferSize];
		newBuffer->pContext			= nullptr;

		m_xaudioBuffers.push_back( newBuffer );
	}

    return true;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::PopulateNextAudioBuffer( OASourcePtr anAudioSource )
{
	XAUDIO2_BUFFER* audioBuffer = GetNextEmptyBuffer();
	if( audioBuffer == nullptr )
	{
		return;
	}

	// work out how much data to read based on the source type
	OAUInt32 bytesToRead = 0, bytesRead = 0, byteOffset = 0;
	switch( m_sourceType )
	{
	case ESourceType::ESourceType_Memory:
		if( m_totalBytesRead == m_totalDataSize )
		{
			return;
		}
		bytesToRead = m_totalDataSize;
		break;

	case ESourceType::ESourceType_Streaming:
		if( m_totalBytesRead == m_totalDataSize )
		{
			return;
		}

		bytesToRead = g_streamingBufferSize;
		byteOffset  = m_totalBytesRead;
		if( m_totalBytesRead + bytesToRead > m_totalDataSize )
		{
			bytesToRead = m_totalDataSize - m_totalBytesRead;
		}
		break;

	case ESourceType::ESourceType_Procedural:
		bytesToRead = g_streamingBufferSize;
		break;
	}

	// read in the data
	anAudioSource->PopulateAudioBuffer( audioBuffer, bytesToRead, bytesRead, byteOffset );
	audioBuffer->AudioBytes = bytesRead;
	audioBuffer->pContext	= nullptr; 
	m_totalBytesRead		+= bytesRead;
}

//////////////////////////////////////////////////////////////////////////

XAUDIO2_BUFFER* COAEVoice::GetNextFullBuffer()
{
	// find and return a buffer with audio data, and without a current context
	for( OAUInt8 i = 0; i < m_xaudioBuffers.size(); ++i )
	{
		if( m_xaudioBuffers[i]->pContext == nullptr && m_xaudioBuffers[i]->AudioBytes > 0 )
		{
			return m_xaudioBuffers[i];
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

XAUDIO2_BUFFER* COAEVoice::GetNextEmptyBuffer()
{
	// find and return a buffer without a context, and without any audio data
	for( OAUInt8 i = 0; i < m_xaudioBuffers.size(); ++i )
	{
		if( m_xaudioBuffers[i]->pContext == nullptr && m_xaudioBuffers[i]->AudioBytes == 0 )
		{
			return m_xaudioBuffers[i];
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////