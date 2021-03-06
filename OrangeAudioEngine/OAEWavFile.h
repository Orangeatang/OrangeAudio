#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OAEFile.h>


//////////////////////////////////////////////////////////////////////////
/// COAEWavFile
//////////////////////////////////////////////////////////////////////////

class COAEWavFile : public IOAEFile
{
public:

    //////////////////////////////////////////////////////////////////////////

    COAEWavFile( const OASourceId& anId, const std::string& aFilePath, ESourceType aSourceType );
    virtual ~COAEWavFile();

    //////////////////////////////////////////////////////////////////////////

    bool	Open() override;
    void    Close() override;

    bool    LoadWaveFormat() override;

    bool    PopulateAudioBuffer( XAUDIO2_BUFFER* anAudioBuffer, OAUInt32 aBytesToRead, OAUInt32& aBytesRead, OAUInt32 aByteOffset = 0 ) override;


private:

    //////////////////////////////////////////////////////////////////////////

    bool LocateChunk( const OAUInt32 aRiffChunkType, OAUInt32& aChunkSize, OAUInt32& aChunkPosition );
    bool ReadChunk( const void* aBuffer, OAUInt32 aBufferSize, OAUInt32 aBufferOffset, OAUInt32& aBytesRead );
};


//////////////////////////////////////////////////////////////////////////