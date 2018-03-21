#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// IOAEAudioSource
//////////////////////////////////////////////////////////////////////////

// the base interface from which all audio sources are derived
class IOAESource
{
public:

    //////////////////////////////////////////////////////////////////////////

    IOAESource( const OASourceId& anId );
    virtual ~IOAESource();

    //////////////////////////////////////////////////////////////////////////

    virtual OAUInt32            PopulateAudioBuffer( XAUDIO2_BUFFER* anAudioBuffer, OAUInt32 aBytesToRead ) = 0;

    //////////////////////////////////////////////////////////////////////////

    const OASourceId&           GetId() const;
    bool                        IsValid() const;
    const WAVEFORMATEXTENSIBLE* GetWaveFormat() const;
    const OAUInt32&             GetDataSize() const;

    //////////////////////////////////////////////////////////////////////////
    
    bool operator==( const OASourceId& aSourceId ) const;


protected:

    //////////////////////////////////////////////////////////////////////////

    void                        SetIsValid( bool anIsValid );

    //////////////////////////////////////////////////////////////////////////

    OASourceId              m_id;
    WAVEFORMATEXTENSIBLE    m_wavFormat;

    OAUInt32                m_dataSize;

    bool                    m_isValid;
};

//////////////////////////////////////////////////////////////////////////