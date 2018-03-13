
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEEmitterObject.h"
#include "OAEVoiceManager.h"
#include "OAEWavFile.h"


//////////////////////////////////////////////////////////////////////////
/// COAEAudioObject
//////////////////////////////////////////////////////////////////////////

COAEEmitterObject::COAEEmitterObject( OAEmitterId anId, COAEVoiceManager* aVoiceManager ) :
    m_id( anId ),
    m_voiceManager( aVoiceManager ),
    m_voice( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////

COAEEmitterObject::~COAEEmitterObject()
{
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COAEEmitterObject::PlaySound( const std::string& anAudioFile, IXAudio2& anAudioInterface )
{
	// test loading a .wav file before hooking code up with file manager
	COAEWavFile* wavFile = new COAEWavFile(1, anAudioFile);
    wavFile->Open();
    wavFile->LoadWaveFormat();
    wavFile->LoadData();

    HRESULT result = anAudioInterface.CreateSourceVoice( &m_voice, (WAVEFORMATEX*)(wavFile->GetWaveFormat()) );
    if( result != S_OK )
    {
        return INVALID_AUDIO_VOICE;
    }

    result = m_voice->SubmitSourceBuffer( wavFile->GetAudioBuffer() );
    if( result != S_OK )
    {
        return INVALID_AUDIO_VOICE;
    }

    result = m_voice->Start();
    if( result != S_OK )
    {
        return INVALID_AUDIO_VOICE;
    }

	return 1;
}

//////////////////////////////////////////////////////////////////////////

OAVoiceId COAEEmitterObject::PlaySound( const OASourceId& aSourceId )
{


    return INVALID_AUDIO_VOICE;
}

//////////////////////////////////////////////////////////////////////////