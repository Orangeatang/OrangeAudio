
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAESourceManager.h"
#include "OAELogger.h"
#include "OAEWavFile.h"


//////////////////////////////////////////////////////////////////////////
/// Globals
//////////////////////////////////////////////////////////////////////////

namespace
{
    static const char* g_wavExtension = "wav";
}


//////////////////////////////////////////////////////////////////////////
/// COAESourceManager
//////////////////////////////////////////////////////////////////////////

COAESourceManager::COAESourceManager() :
    m_nextSourceId( INVALID_AUDIO_SOURCE + 1 )
{
}

//////////////////////////////////////////////////////////////////////////

COAESourceManager::~COAESourceManager()
{
}

//////////////////////////////////////////////////////////////////////////

OASourceId COAESourceManager::AddSource( const std::string& aFileName )
{   
    // find the file extension
    size_t extensionIndex = aFileName.find_last_of( "." );
    if( extensionIndex == std::string::npos )
    {
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "COAESourceManager::AddSource - Unable to detect file type: %s", aFileName.c_str() );
        return INVALID_AUDIO_SOURCE;
    }

    std::string extension = aFileName.substr( extensionIndex + 1 );
    if( extension.size() == 0 )
    {
         OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "COAESourceManager::AddSource - Unable to detect file type: %s", aFileName.c_str() );
         return INVALID_AUDIO_SOURCE;
    }

    // create the appropriate source instance based on the file type
    std::transform( extension.begin(), extension.end(), extension.begin(), ::tolower ); 
    if( extension == g_wavExtension )
    {
        OASourcePtr newSource( new COAEWavFile(m_nextSourceId++) );
        m_fileSources.push_back( newSource );

        return newSource->GetId();
    }

    return INVALID_AUDIO_SOURCE;
}

//////////////////////////////////////////////////////////////////////////