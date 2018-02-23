
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEAudioObject.h"

#include "OAEWavFile.h"


//////////////////////////////////////////////////////////////////////////
/// COAEAudioObject
//////////////////////////////////////////////////////////////////////////

COAEAudioObject::COAEAudioObject( OAInt64 anId ) :
    m_id( anId ),
    m_voice( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////

COAEAudioObject::~COAEAudioObject()
{
}

//////////////////////////////////////////////////////////////////////////

OAInt32 COAEAudioObject::PlaySound( const std::string& anAudioFile, IXAudio2& anAudioInterface )
{
	// test loading a .wav file before hooking code up with file manager
	COAEWavFile* wavFile = new COAEWavFile(1);
	wavFile->LoadFile(anAudioFile);
	//delete wavFile;

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