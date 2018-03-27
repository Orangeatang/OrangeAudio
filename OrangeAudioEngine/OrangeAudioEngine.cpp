
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OrangeAudioEngine.h"
#include "OAELogger.h"
#include "OAEEmitterObject.h"
#include "OAESourceManager.h"
#include "OAEVoiceManager.h"


//////////////////////////////////////////////////////////////////////////
/// COrangeAudioEngine
//////////////////////////////////////////////////////////////////////////

COrangeAudioEngine::COrangeAudioEngine() :
    m_xaudioInterface( nullptr ),
    m_sourceManager( nullptr ),
    m_initialized( false )
{
}

//////////////////////////////////////////////////////////////////////////

COrangeAudioEngine::~COrangeAudioEngine()
{
}

//////////////////////////////////////////////////////////////////////////

bool COrangeAudioEngine::Initialize()
{
    // initialize the low level audio
    if( !InitializeXAudio2() )
    {
        return false;
    }

    // initialize the engine managers
    if( !InitializeManagers() )
    {
        return false;
    }

    OAELog->LogMessage( ELogMesageType::ELogMessageType_Info, "OrangeAudioEngine initialization complete");

    m_initialized = true;
    return true;
}

//////////////////////////////////////////////////////////////////////////

void COrangeAudioEngine::Uninitialize()
{
    if( !m_initialized )
    {
        return;
    }

    Cleanup();

    m_xaudioMasteringVoice->DestroyVoice();
    m_xaudioMasteringVoice = nullptr;

    m_xaudioInterface->Release();
    m_xaudioInterface = nullptr;

    // make sure we clean up the logger
    OAELog->Destroy();
}

//////////////////////////////////////////////////////////////////////////

bool COrangeAudioEngine::RegisterEmitter( const OAEmitterId& anId )
{
    if( !m_initialized )
    {
        return false;
    }

    // make sure the emitter doesn't already exist
    if( m_emitters.find(anId) != m_emitters.end() )
    {
        return false;
    }

    OAEmitterPtr newEmitter( new COAEEmitterObject(anId, m_voiceManager) );
    m_emitters[anId] = newEmitter;

    return true;
}

//////////////////////////////////////////////////////////////////////////

void COrangeAudioEngine::UnregisterEmitter( const OAEmitterId& anId )
{
    if( !m_initialized )
    {
        return;
    }

    auto emitter = m_emitters.find( anId );
    if( emitter != m_emitters.end() )
    {
        emitter->second = nullptr;
        m_emitters.erase( emitter->first );
    }
}

//////////////////////////////////////////////////////////////////////////

bool COrangeAudioEngine::RegisterListener( const OAListenerId& anId )
{
    if( !m_initialized )
    {
        return false;
    }

    // make sure the listener doesn't already exist;;;;
    if( m_listeners.find(anId) != m_listeners.end() )
    {
        return false;
    }

    OAEmitterPtr newListener( new COAEEmitterObject(anId, m_voiceManager) );
    m_listeners[anId] = newListener;

    return true;
}

//////////////////////////////////////////////////////////////////////////

void COrangeAudioEngine::UnregisterListener( const OAListenerId& anId )
{
    if( !m_initialized )
    {
        return;
    }

    auto listener = m_listeners.find( anId );
    if( listener != m_listeners.end() )
    {
        listener->second = nullptr;
        m_listeners.erase( listener->first );
    }
}

//////////////////////////////////////////////////////////////////////////

OASourceId COrangeAudioEngine::AddSource( const std::string& aFileName, bool anIsStreaming )
{
    if( !m_initialized )
    {
        return INVALID_AUDIO_SOURCE;
    }

    return m_sourceManager->AddFileSource( aFileName, anIsStreaming );
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COrangeAudioEngine::PlaySound( const OAEmitterId& anEmitterId, const std::string& anAudioFile, bool anIsStreaming /* = false*/ )
{
	if( !m_initialized )
	{
		return INVALID_AUDIO_VOICE;
	}

	OAEmitterPtr emitter = GetEmitter( anEmitterId );
	if( !emitter )
	{
		return INVALID_AUDIO_VOICE;
	}

    OASourceId sourceId = m_sourceManager->AddFileSource( anAudioFile, anIsStreaming );
    if( sourceId == INVALID_AUDIO_SOURCE )
    {
        return INVALID_AUDIO_VOICE;
    }

	return emitter->PlaySound( sourceId );
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COrangeAudioEngine::PlaySound( const OAEmitterId& anEmitterId, const OASourceId& aSourceId )
{
    if( !m_initialized )
    {
        return INVALID_AUDIO_VOICE;
    }

    OAEmitterPtr emitter = GetEmitter( anEmitterId );
    if( !emitter )
    {
        return INVALID_AUDIO_VOICE;
    }

    return emitter->PlaySound( aSourceId );
}

//////////////////////////////////////////////////////////////////////////

void COrangeAudioEngine::Update( OAFloat32 aDeltaTime )
{
    if( !m_initialized )
    {
        return;
    }

    UpdateManagers( aDeltaTime );
}

//////////////////////////////////////////////////////////////////////////

OAEmitterPtr COrangeAudioEngine::GetEmitter( const OAEmitterId& anEmitterId )
{
	auto emitter = m_emitters.find( anEmitterId );
	if( emitter != m_emitters.end() )
	{
		return emitter->second;
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

bool COrangeAudioEngine::InitializeXAudio2()
{
    HRESULT result = S_FALSE;

    // initialize
    CoInitializeEx( NULL, COINIT_MULTITHREADED );

    // create the xaudio2 interface
    result = XAudio2Create( &m_xaudioInterface, 0, XAUDIO2_DEFAULT_PROCESSOR );
    if (result != S_OK)
    {
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "Unable to create XAudio2 interface: %l", result );
        return false;
    }

    // create the mastering voice
    result = m_xaudioInterface->CreateMasteringVoice( &m_xaudioMasteringVoice );
    if (result != S_OK)
    {
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "Unable to create mastering voice: %l", result );
        return false;
    }

    // the mastering voice in xaudio2 v2.7 (DXSDK) doesn't support GetChannelMask
    DWORD channelMask = 1;
#if !USE_DXSDK
    m_xaudioMasteringVoice->GetChannelMask(&channelMask);
#else
#endif
    X3DAudioInitialize( channelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3DInstance );

    return true;
}

//////////////////////////////////////////////////////////////////////////

bool COrangeAudioEngine::InitializeManagers()
{
    m_sourceManager = new COAESourceManager();
    m_voiceManager  = new COAEVoiceManager( m_sourceManager, m_xaudioInterface );
    return true;
}

//////////////////////////////////////////////////////////////////////////

void COrangeAudioEngine::UpdateManagers( OAFloat32 aDeltaTime )
{
    m_voiceManager->Update( aDeltaTime );
}

//////////////////////////////////////////////////////////////////////////

void COrangeAudioEngine::Cleanup()
{
    // clean up the emitters
    for( auto iterator = m_emitters.begin(); iterator != m_emitters.end(); ++iterator )
    {
        iterator->second = nullptr;
    }
    m_emitters.clear();

    // clean up the listeners
    for( auto iterator = m_listeners.begin(); iterator != m_listeners.end(); ++iterator )
    {
        iterator->second = nullptr;
    }
    m_emitters.clear();

    // delete the various managers
    delete m_sourceManager;
    delete m_voiceManager;
}

//////////////////////////////////////////////////////////////////////////