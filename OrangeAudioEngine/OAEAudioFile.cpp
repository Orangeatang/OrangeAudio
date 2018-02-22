
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEAudioFile.h"


//////////////////////////////////////////////////////////////////////////
/// IAudioFile
//////////////////////////////////////////////////////////////////////////

IOAEAudioFile::IOAEAudioFile( const OAUInt64& anId ) :
    m_id( anId ),
    m_wavFormat( {0} ),
    m_buffer( {0} )
{
}

//////////////////////////////////////////////////////////////////////////

IOAEAudioFile::~IOAEAudioFile()
{
}

//////////////////////////////////////////////////////////////////////////

const OAUInt64& IOAEAudioFile::GetId() const
{
    return m_id;
}

//////////////////////////////////////////////////////////////////////////