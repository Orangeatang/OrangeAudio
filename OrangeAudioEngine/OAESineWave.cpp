
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAESineWave.h"
#include "OAELogger.h"

#include <intsafe.h>


//////////////////////////////////////////////////////////////////////////
/// COAESineWave
//////////////////////////////////////////////////////////////////////////

COAESineWave::COAESineWave( const OASourceId& aSourceId ) : IOAEProceduralSource( aSourceId ),
	m_currentTime( 0.0f ),
	m_deltaTime( 0.0f ),
	m_frequency( 400.0f ),
	m_sampleRate( 44100 ),
	m_bytesPerChannel( 2 ),
	m_channelCount( 2 ),
	m_power( SHORT_MAX )
{
}

//////////////////////////////////////////////////////////////////////////

COAESineWave::~COAESineWave()
{
}

//////////////////////////////////////////////////////////////////////////

bool COAESineWave::Initialize()
{
	// paramaters for the wav format
	m_wavFormat.Format.cbSize			= sizeof(WAVEFORMATEX);
	m_wavFormat.Format.wFormatTag		= WAVE_FORMAT_PCM;
	m_wavFormat.Format.nChannels		= m_channelCount;
	m_wavFormat.Format.nSamplesPerSec	= m_sampleRate;
	m_wavFormat.Format.wBitsPerSample	= m_bytesPerChannel * 8;
	m_wavFormat.Format.nBlockAlign		= m_wavFormat.Format.nChannels * (m_wavFormat.Format.wBitsPerSample / 8);
	m_wavFormat.Format.nAvgBytesPerSec	= m_wavFormat.Format.nBlockAlign * m_wavFormat.Format.nSamplesPerSec;

	// parameters for the sine wave
	m_currentTime						= 0.0f;
	m_deltaTime							= 1.0f / m_wavFormat.Format.nSamplesPerSec;
	m_frequency							= 440.0f;

	// set the volume/power multiplier for the sine wave.
	// the sin function returns a value between -1.0f : 1.0f
	// because the audio signal is in bytes we need convert the 
	// sine value to a different range.
	// to accomplish this we multiply the sin value by the max value for the data type.
	switch( m_bytesPerChannel )
	{
	case 1:
		m_power = BYTE_MAX;
		break;

	case 2:
		m_power = SHORT_MAX;
		break;

	default:
		OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "COAESineWave::Initialize: Unsupported bytes per channel configuration [%d]", m_bytesPerChannel );
		return false;
	}

	SetIsValid( true );
	return true;
}

//////////////////////////////////////////////////////////////////////////

// the sine wave generation uses the following formula:
//	y = m_power * sin( 2pi * m_frequency * m_time )
// where m_power is the max value of the data type
bool COAESineWave::PopulateAudioBuffer( XAUDIO2_BUFFER* anAudioBuffer, OAUInt32 aBytesToRead, OAUInt32& aBytesRead, OAUInt32 aByteOffset )
{
	OAUInt32 sampleCount = aBytesToRead / (m_channelCount * m_bytesPerChannel);
	switch( m_bytesPerChannel )
	{
	case 1:
		PopulateByteBuffer( anAudioBuffer, sampleCount );
		break;

	case 2:
		PopulateShortBuffer( anAudioBuffer, sampleCount );
		break;

	default:
		OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "COAESineWave::PopulateAudiobuffer: Unsupported bytes-per-channel configuration [%d]", m_bytesPerChannel );
		aBytesRead = 0;
		return false;
	}

	aBytesRead = aBytesToRead;
	return true;
}

//////////////////////////////////////////////////////////////////////////

void COAESineWave::PopulateByteBuffer( XAUDIO2_BUFFER* anAudioBuffer, OAUInt32 aSampleCount )
{
	static const OAFloat32 twoPi = (OAFloat32)M_PI * 2.0f;

	OAInt8* currentSample = (OAInt8*)anAudioBuffer->pAudioData;
	for( OAUInt32 i = 0; i < aSampleCount; ++i )
	{
		currentSample[0] = (OAInt8)(m_power * sin(twoPi * m_frequency * m_currentTime));
		for( OAUInt32 y = m_channelCount - 1; y > 0; --y )
		{
			currentSample[y] = currentSample[0];
		}

		currentSample += m_channelCount;
		m_currentTime += m_deltaTime;
	}
}

//////////////////////////////////////////////////////////////////////////

void COAESineWave::PopulateShortBuffer( XAUDIO2_BUFFER* anAudioBuffer, OAUInt32 aSampleCount )
{	
	static const OAFloat32 twoPi = (OAFloat32)M_PI * 2.0f;

	OAInt16* currentSample	= (OAInt16*)anAudioBuffer->pAudioData;
	for( OAUInt32 i = 0; i < aSampleCount; ++i )
	{
		currentSample[0] = (OAInt16)(m_power * sin(twoPi * m_frequency * m_currentTime));
		for( OAUInt32 y = m_channelCount - 1; y > 0; --y )
		{
			currentSample[y] = currentSample[0];
		}

		currentSample += m_channelCount;
		m_currentTime += m_deltaTime;
	}
}

//////////////////////////////////////////////////////////////////////////