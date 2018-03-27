
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEFile.h"


//////////////////////////////////////////////////////////////////////////
/// IAudioFile
//////////////////////////////////////////////////////////////////////////

IOAEFile::IOAEFile( const OASourceId& anId, const std::string& aFilePath, ESourceType aSourceType ) : IOAESource( anId, aSourceType ),
    m_filePath( aFilePath )
{
}

//////////////////////////////////////////////////////////////////////////

IOAEFile::~IOAEFile()
{
}

//////////////////////////////////////////////////////////////////////////