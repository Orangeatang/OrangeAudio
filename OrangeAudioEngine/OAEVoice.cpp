
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

COAEVoice::COAEVoice( const OAVoiceId& aVoiceId, const OASourceId& aSourceId, bool isStreaming /* = false*/ ) :
    m_id( aVoiceId ),
    m_audioSourceId( aSourceId ),
    m_isStreaming( isStreaming ),
    m_state( EVoiceState::EVoiceState_Invalid ),
    m_voice( nullptr ),
    m_audioBuffers( nullptr ),
    m_activeBuffer( 0 ),
    m_bufferReady( false )
{
}

//////////////////////////////////////////////////////////////////////////

COAEVoice::~COAEVoice()
{
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
    if( !InitializeBuffers(audioSource->GetDataSize()) )
    {
        return false;
    }

    m_state = EVoiceState::EVoiceState_Ready;
    return true;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::Uninitialize()
{
}

//////////////////////////////////////////////////////////////////////////

bool COAEVoice::Play()
{
    if( !m_voice )
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
    if( !m_voice )
    {
        return;
    }

    m_voice->Stop();
    m_state = EVoiceState::EVoiceState_Stopped;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::PrepareBuffers( COAESourceManager* aSourceManager )
{
    if( m_audioBuffers == nullptr || m_bufferReady )
    {
        return;
    }

    OASourcePtr audioSource = aSourceManager->GetSource( m_audioSourceId );
    if( audioSource == nullptr )
    {
        Stop();
        return;
    }

	OAUInt32 bytesRead = 0;
    audioSource->PopulateAudioBuffer( m_audioBuffers, audioSource->GetDataSize(), bytesRead );
    m_audioBuffers->AudioBytes = bytesRead;

    m_bufferReady = true;
}

//////////////////////////////////////////////////////////////////////////

const OAVoiceId& COAEVoice::GetId() const
{
    return m_id;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoice::OnVoiceProcessingPassStart( UINT32 aSamplesRequired )
{
    static bool done = false;

    // create and submit the XAUDIO_BUFFER
    if( m_bufferReady && !done )
    {
        HRESULT result = m_voice->SubmitSourceBuffer( m_audioBuffers );
        done = true;
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
    if( m_isStreaming )
    {
        m_state = EVoiceState::EVoiceState_Stopped;
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

bool COAEVoice::InitializeBuffers( OAUInt32 aBufferSize )
{
    if( m_isStreaming )
    {
        m_audioBuffers                  = new XAUDIO2_BUFFER[g_streamingBufferCount];
        m_audioBuffers[0].pAudioData    = new BYTE[g_streamingBufferSize];
        m_audioBuffers[1].pAudioData    = new BYTE[g_streamingBufferSize];
    }
    else
    {
        if( aBufferSize == 0 )
        {
            return false;
        }

        m_audioBuffers                  = new XAUDIO2_BUFFER( {0} );
        m_audioBuffers->pAudioData      = new BYTE[aBufferSize];
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////