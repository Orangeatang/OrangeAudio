#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// IOAEAudioSource
//////////////////////////////////////////////////////////////////////////

// the base interface from which all audio sources are derived
class IOAEAudioSource
{
public:

    //////////////////////////////////////////////////////////////////////////

    IOAEAudioSource( const OAUInt64& anId );
    virtual ~IOAEAudioSource();


protected:

    //////////////////////////////////////////////////////////////////////////

    OAUInt64                m_id;
    WAVEFORMATEXTENSIBLE    m_wavFormat;
};

//////////////////////////////////////////////////////////////////////////