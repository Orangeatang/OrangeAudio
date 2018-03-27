#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>
#include <OAESource.h>


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

    COAEVoice( const OAVoiceId& aVoiceId, const OASourceId& aSourceId );
    virtual ~COAEVoice();

    //////////////////////////////////////////////////////////////////////////

    bool                    Initialize( IXAudio2* anXaudio2Interface, COAESourceManager* aSourceManager );
    void                    Uninitialize();

    //////////////////////////////////////////////////////////////////////////

    bool                    Play();
    void                    Stop();

    void                    Update( COAESourceManager* aSourceManager );

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

    bool                    InitializeAudioBuffers();
	void					PopulateNextAudioBuffer( OASourcePtr anAudioSource );
	
	XAUDIO2_BUFFER*			GetNextFullBuffer();
	XAUDIO2_BUFFER*			GetNextEmptyBuffer();

    //////////////////////////////////////////////////////////////////////////

	OAVoiceId						m_id;
	OASourceId						m_audioSourceId;
	ESourceType						m_sourceType;

	EVoiceState						m_state;

	IXAudio2SourceVoice*			m_voice;
	std::vector<XAUDIO2_BUFFER*>	m_xaudioBuffers;

	OAUInt32						m_totalDataSize;
	OAUInt32						m_totalBytesRead;
};