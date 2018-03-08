
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEFile.h"


//////////////////////////////////////////////////////////////////////////
/// IAudioFile
//////////////////////////////////////////////////////////////////////////

IOAEFile::IOAEFile( const OASourceId& anId ) : IOAESource( anId ),
    m_xaudioBuffer( {0} ),
    m_dataBuffer( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////

IOAEFile::~IOAEFile()
{
    delete m_dataBuffer;
}

//////////////////////////////////////////////////////////////////////////

const XAUDIO2_BUFFER* IOAEFile::GetAudioBuffer() const
{
    return &m_xaudioBuffer;
}

//////////////////////////////////////////////////////////////////////////

const OAUInt8* IOAEFile::GetAudioData() const
{
    return m_dataBuffer;
}

//////////////////////////////////////////////////////////////////////////