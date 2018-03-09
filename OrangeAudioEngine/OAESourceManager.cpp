
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
    OASourcePtr newSource = nullptr;
    if( extension == g_wavExtension )
    {
        newSource = CreateWavFileSource( aFileName );
    }

    // add the source
    if( newSource != nullptr )
    {
        m_fileSources.push_back( newSource );
        return newSource->GetId();
    }

    return INVALID_AUDIO_SOURCE;
}

//////////////////////////////////////////////////////////////////////////

OASourcePtr COAESourceManager::CreateWavFileSource( const std::string& aFileName )
{
    // create the wav file
    COAEWavFile* newWavFile = new COAEWavFile(m_nextSourceId++, aFileName);
    if( newWavFile == nullptr )
    {
        return nullptr;
    }

    // open and validate the .wav
    if( !newWavFile->Open() )
    {
        delete newWavFile;
        return nullptr;
    }

    // load the wav format
    if( !newWavFile->LoadWaveFormat() )
    {
        delete newWavFile;
        return nullptr;
    }

    return OASourcePtr( newWavFile );
}

//////////////////////////////////////////////////////////////////////////