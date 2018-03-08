#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// COAESound
//////////////////////////////////////////////////////////////////////////

// COAEVoiceInstance represents an instance of a sound asset playing back through the master voice
class COAEVoice : public IXAudio2VoiceCallback
{
public:

    //////////////////////////////////////////////////////////////////////////

    COAEVoice();
    virtual ~COAEVoice();

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
    OASourceId              m_audioSourceId;
    OAUInt8**               m_audioBuffers;
};