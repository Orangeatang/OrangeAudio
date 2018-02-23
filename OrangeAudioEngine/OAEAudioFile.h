#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// COAEAudioFile
//////////////////////////////////////////////////////////////////////////

// abstract base class for all audio file types
class IOAEAudioFile
{
public:

    //////////////////////////////////////////////////////////////////////////

    IOAEAudioFile( const OAUInt64& anId );
    virtual ~IOAEAudioFile();

    //////////////////////////////////////////////////////////////////////////

    virtual bool    LoadFile( const std::string& aFilePath ) = 0;

    //////////////////////////////////////////////////////////////////////////

    const OAUInt64& GetId() const;


protected:

    //////////////////////////////////////////////////////////////////////////

    OAUInt64                m_id;

	std::fstream			m_fileStream;

    WAVEFORMATEXTENSIBLE    m_wavFormat;
    XAUDIO2_BUFFER          m_xaudioBuffer;
    OAUInt8*                m_dataBuffer;
};

//////////////////////////////////////////////////////////////////////////