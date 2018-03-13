
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEVoiceManager.h"
#include "OAESourceManager.h"


//////////////////////////////////////////////////////////////////////////
/// COAEVoiceManager
//////////////////////////////////////////////////////////////////////////

COAEVoiceManager::COAEVoiceManager(  COAESourceManager* aSourceManager ) :
    m_sourceManager( aSourceManager ),
    m_nextVoiceId( INVALID_AUDIO_VOICE + 1 )
{
}

//////////////////////////////////////////////////////////////////////////

COAEVoiceManager::~COAEVoiceManager()
{
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COAEVoiceManager::CreateVoice( const OASourceId& aSourceId )
{
    if( !m_sourceManager->IsValid(aSourceId) )
    {
        return INVALID_AUDIO_VOICE;
    }

    return INVALID_AUDIO_VOICE;
}

//////////////////////////////////////////////////////////////////////////

void COAEVoiceManager::DestroyVoice( const OAVoiceId& aVoiceId )
{
}

//////////////////////////////////////////////////////////////////////////