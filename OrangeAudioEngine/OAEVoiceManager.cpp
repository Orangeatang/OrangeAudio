
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEVoiceManager.h"
#include "OAESourceManager.h"
#include "OAEVoice.h"


//////////////////////////////////////////////////////////////////////////
/// COAEVoiceManager
//////////////////////////////////////////////////////////////////////////

COAEVoiceManager::COAEVoiceManager(  COAESourceManager* aSourceManager, IXAudio2* anXAudio2Interface ) :
    m_sourceManager( aSourceManager ),
    m_xaudio2Interface( anXAudio2Interface ),
    m_nextVoiceId( INVALID_AUDIO_VOICE + 1 )
{
}

//////////////////////////////////////////////////////////////////////////

COAEVoiceManager::~COAEVoiceManager()
{
    for( auto voice : m_voices )
    {
        voice.second->Stop();
        voice.second->Uninitialize();
        voice.second = nullptr;
    }
    m_voices.clear();
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COAEVoiceManager::CreateVoice( const OASourceId& aSourceId )
{
    if( !m_sourceManager->IsValid(aSourceId) )
    {
        return INVALID_AUDIO_VOICE;
    }

    // create and initialize the new voice
    COAEVoice* newVoice = new COAEVoice(m_nextVoiceId++, aSourceId);
    if( !newVoice->Initialize(m_xaudio2Interface, m_sourceManager) )
    {
        delete newVoice;
        return INVALID_AUDIO_SOURCE;
    }
    
    // track the new voice
    m_voices[newVoice->GetId()] = OAVoicePtr( newVoice );
    return newVoice->GetId();
}

//////////////////////////////////////////////////////////////////////////

void COAEVoiceManager::DestroyVoice( const OAVoiceId& aVoiceId )
{
    auto iterator = m_voices.find( aVoiceId );
    if( iterator != m_voices.end() )
    {
        iterator->second->Stop();
        iterator->second->Uninitialize();
        iterator->second = nullptr;

        m_voices.erase( iterator );
    }
}

//////////////////////////////////////////////////////////////////////////

bool COAEVoiceManager::Play( const OAVoiceId& aVoiceId )
{
    auto iterator = m_voices.find( aVoiceId );
    if( iterator != m_voices.end() )
    {
        iterator->second->Play();
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////

// populate voice buffers, ready to feed in to the xaudio thread
void COAEVoiceManager::Update( OAFloat32 aDeltaTime )
{
    for( auto voice : m_voices )
    {
        voice.second->Update( m_sourceManager );
    }
}

//////////////////////////////////////////////////////////////////////////