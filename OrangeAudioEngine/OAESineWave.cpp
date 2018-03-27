
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAESineWave.h"


//////////////////////////////////////////////////////////////////////////
/// COAESineWave
//////////////////////////////////////////////////////////////////////////

COAESineWave::COAESineWave( const OASourceId& aSourceId ) : IOAEProceduralSource( aSourceId ),
	m_amplitude( 1.0f ),
	m_currentTime( 0.0f ),
	m_deltaTime( 0.0f ),
	m_frequency( 400.0f )
{
}

//////////////////////////////////////////////////////////////////////////

COAESineWave::~COAESineWave()
{
}

//////////////////////////////////////////////////////////////////////////

bool COAESineWave::Initialize()
{
	m_wavFormat.Format.cbSize			= sizeof(WAVEFORMATEX);
	m_wavFormat.Format.wFormatTag		= WAVE_FORMAT_PCM;
	m_wavFormat.Format.nChannels		= 2;
	m_wavFormat.Format.nSamplesPerSec	= 44100;
	m_wavFormat.Format.wBitsPerSample	= 16;
	m_wavFormat.Format.nBlockAlign		= m_wavFormat.Format.nChannels * (m_wavFormat.Format.wBitsPerSample / 8);
	m_wavFormat.Format.nAvgBytesPerSec	= m_wavFormat.Format.nBlockAlign * m_wavFormat.Format.nSamplesPerSec;

	m_amplitude							= 1.0f;
	m_currentTime						= 0.0f;
	m_deltaTime							= 1.0f / m_wavFormat.Format.nSamplesPerSec;
	m_frequency							= 400.0f;

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool COAESineWave::PopulateAudioBuffer( XAUDIO2_BUFFER* anAudioBuffer, OAUInt32 aBytesToRead, OAUInt32& aBytesRead, OAUInt32 aByteOffset )
{
	return true;
}

//////////////////////////////////////////////////////////////////////////