
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAESource.h"


//////////////////////////////////////////////////////////////////////////
/// IOAEAudioSource
//////////////////////////////////////////////////////////////////////////

IOAESource::IOAESource( const OASourceId& anId, ESourceType aSourceType /* = ESourceType::ESourceType_Memory*/ ) :
    m_id( anId ),
	m_type( aSourceType ),
    m_isValid( false ),
    m_wavFormat( {0} ),
    m_dataSize( 0 )
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

ESourceType IOAESource::GetType() const
{
	return m_type;
}

//////////////////////////////////////////////////////////////////////////

const WAVEFORMATEXTENSIBLE* IOAESource::GetWaveFormat() const
{
    return &m_wavFormat;
}

//////////////////////////////////////////////////////////////////////////

const OAUInt32& IOAESource::GetDataSize() const
{
    return m_dataSize;
}

//////////////////////////////////////////////////////////////////////////

bool IOAESource::IsValid() const
{
	return m_isValid;
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