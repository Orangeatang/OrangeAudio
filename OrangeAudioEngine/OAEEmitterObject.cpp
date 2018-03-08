
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEEmitterObject.h"

#include "OAEWavFile.h"


//////////////////////////////////////////////////////////////////////////
/// COAEAudioObject
//////////////////////////////////////////////////////////////////////////

COAEEmitterObject::COAEEmitterObject( OAEmitterId anId ) :
    m_id( anId ),
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
	COAEWavFile* wavFile = new COAEWavFile(1);
	wavFile->Load(anAudioFile);

    HRESULT result = anAudioInterface.CreateSourceVoice( &m_voice, (WAVEFORMATEX*)(wavFile->GetWaveFormat()) );
    if( result != S_OK )
    {
        return 0;
    }

    result = m_voice->SubmitSourceBuffer( wavFile->GetAudioBuffer() );
    if( result != S_OK )
    {
        return 0;
    }

    result = m_voice->Start();
    if( result != S_OK )
    {
        return 0;
    }

	return 1;
}

//////////////////////////////////////////////////////////////////////////