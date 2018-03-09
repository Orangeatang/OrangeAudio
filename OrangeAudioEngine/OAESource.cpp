
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAESource.h"


//////////////////////////////////////////////////////////////////////////
/// IOAEAudioSource
//////////////////////////////////////////////////////////////////////////

IOAESource::IOAESource( const OASourceId& anId ) :
    m_id( anId ),
    m_isValid( false ),
    m_wavFormat( {0} )
{
}

//////////////////////////////////////////////////////////////////////////

IOAESource::~IOAESource()
{
}

//////////////////////////////////////////////////////////////////////////

const OASourceId& IOAESource::GetId() const
{
    return m_id;
}

//////////////////////////////////////////////////////////////////////////

bool IOAESource::IsValid() const
{
    return m_isValid;
}

//////////////////////////////////////////////////////////////////////////

const WAVEFORMATEXTENSIBLE* IOAESource::GetWaveFormat() const
{
    return &m_wavFormat;
}

//////////////////////////////////////////////////////////////////////////

bool IOAESource::operator==( const OASourceId& aSourceId ) const
{
    return m_id == aSourceId;
}

//////////////////////////////////////////////////////////////////////////

void IOAESource::SetIsValid( bool anIsValid )
{
    m_isValid = anIsValid;
}

//////////////////////////////////////////////////////////////////////////