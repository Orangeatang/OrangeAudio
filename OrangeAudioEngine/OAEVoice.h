#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// Forward Declarations
//////////////////////////////////////////////////////////////////////////

class COAESourceManager;


//////////////////////////////////////////////////////////////////////////
/// Enumerations
//////////////////////////////////////////////////////////////////////////

enum class EVoiceState : OAUInt8
{
    EVoiceState_Invalid,
    EVoiceState_Ready,
    EVoiceState_Playing,
    EVoiceState_Paused,
    EVoiceState_Stopped
};


//////////////////////////////////////////////////////////////////////////
/// COAESound
//////////////////////////////////////////////////////////////////////////

// COAEVoiceInstance represents an instance of a sound asset playing back through the master voice
class COAEVoice : public IXAudio2VoiceCallback
{
public:

    //////////////////////////////////////////////////////////////////////////

    COAEVoice( const OAVoiceId& aVoiceId, const OASourceId& aSourceId, bool isStreaming = false );
    virtual ~COAEVoice();

    //////////////////////////////////////////////////////////////////////////

    bool                    Initialize( IXAudio2* anXaudio2Interface, COAESourceManager* aSourceManager );
    void                    Uninitialize();

    //////////////////////////////////////////////////////////////////////////

    bool                    Play();
    void                    Stop();

    void                    PrepareBuffers( COAESourceManager* aSourceManager );

    //////////////////////////////////////////////////////////////////////////

    const OAVoiceId&        GetId() const;

    //////////////////////////////////////////////////////////////////////////

    // overridden from IXAudio2VoiceCallback
    void STDMETHODCALLTYPE  OnVoiceProcessingPassStart( UINT32 aSamplesRequired ) override;
    void STDMETHODCALLTYPE  OnVoiceProcessingPassEnd() override;

    void STDMETHODCALLTYPE  OnStreamEnd() override;

    void STDMETHODCALLTYPE  OnBufferStart( void* aBufferContext ) override;
    void STDMETHODCALLTYPE  OnBufferEnd( void* aBufferContext ) override;

    void STDMETHODCALLTYPE  OnLoopEnd( void* aBufferContext ) override;

    void STDMETHODCALLTYPE  OnVoiceError( void* aBufferContext, HRESULT anError );


private:

    //////////////////////////////////////////////////////////////////////////

    bool                    InitializeBuffers( OAUInt32 aBufferSize );

    //////////////////////////////////////////////////////////////////////////

    OAVoiceId               m_id;
    OASourceId              m_audioSourceId;
    bool                    m_isStreaming;

    EVoiceState             m_state;

    IXAudio2SourceVoice*    m_voice;
    XAUDIO2_BUFFER*         m_audioBuffers;
    OAUInt8                 m_activeBuffer;

    bool                    m_bufferReady;
};