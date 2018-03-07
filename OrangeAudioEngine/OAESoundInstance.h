#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// COAESound
//////////////////////////////////////////////////////////////////////////

// COAESoundInstance handles the submission of audio buffers read/streamed in from audio files
// to the xaudio2 voice
class COAESoundInstance : public IXAudio2VoiceCallback
{
public:

    //////////////////////////////////////////////////////////////////////////

    COAESoundInstance();
    virtual ~COAESoundInstance();

    //////////////////////////////////////////////////////////////////////////

    // overridden from IXAudio2VoiceCallback
    void STDMETHODCALLTYPE  OnVoiceProcessingPassStart( UINT32 aSamplesRequired ) override;
    void STDMETHODCALLTYPE  OnVoiceProcessingPassEnd() override;

    void STDMETHODCALLTYPE  OnBufferStart( void* aBufferContext ) override;
    void STDMETHODCALLTYPE  OnBufferEnd( void* aBufferContext ) override;

    void STDMETHODCALLTYPE  OnLoopEnd( void* aBufferContext ) override;

    void STDMETHODCALLTYPE  OnVoiceError( void* aBufferContext, HRESULT anError );


private:

    //////////////////////////////////////////////////////////////////////////

    IXAudio2SourceVoice*    m_voice;
    OAUInt64                m_audioSourceId;
    OAUInt8**               m_audioBuffers;
};