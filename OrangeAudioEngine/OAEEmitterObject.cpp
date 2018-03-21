
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEEmitterObject.h"
#include "OAEVoiceManager.h"
#include "OAEWavFile.h"


//////////////////////////////////////////////////////////////////////////
/// COAEAudioObject
//////////////////////////////////////////////////////////////////////////

COAEEmitterObject::COAEEmitterObject( OAEmitterId anId, COAEVoiceManager* aVoiceManager ) :
    m_id( anId ),
    m_voiceManager( aVoiceManager ),
    m_voice( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////

COAEEmitterObject::~COAEEmitterObject()
{
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COAEEmitterObject::PlaySound( const OASourceId& aSourceId )
{
    // create the voice
    OAVoiceId newVoice = m_voiceManager->CreateVoice( aSourceId );
    if( newVoice == INVALID_AUDIO_VOICE )
    {
        return INVALID_AUDIO_VOICE;
    }

    // try and play the voice
    if( !m_voiceManager->Play(newVoice) ) 
    {
        m_voiceManager->DestroyVoice( newVoice );
        return INVALID_AUDIO_VOICE;
    }

    // track the voice
    m_voices.push_back( newVoice );
    return newVoice;
}

//////////////////////////////////////////////////////////////////////////