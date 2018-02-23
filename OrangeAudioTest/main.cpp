
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngine.h>


//////////////////////////////////////////////////////////////////////////
/// Globals
//////////////////////////////////////////////////////////////////////////

static const OAInt64 g_defaultEmitter	= 0;
static const OAInt64 g_defaultListener	= 0;


//////////////////////////////////////////////////////////////////////////
/// Application Entry Point
//////////////////////////////////////////////////////////////////////////

int main()
{
	// create and initialize the audio engine
	COrangeAudioEngine* audioEngine = new COrangeAudioEngine();
	if( !audioEngine->Initialize() )
	{
		delete audioEngine;
		return -1;
	}

	// register an emitter
	if( !audioEngine->RegisterEmitter(g_defaultEmitter) )
	{
		delete audioEngine;
		return -1;
	}
	
	// register a listener
	if( !audioEngine->RegisterListener(g_defaultListener) )
	{
		delete audioEngine;
		return -1;
	}

	// play a sound
	audioEngine->PlaySound( g_defaultEmitter, "Data/RolandRhumba.wav" );

	// pause in case we want to inspect the COrangeAudioEngine instance
	// system( "pause" );

	audioEngine->Uninitialize();
	delete audioEngine;
	
	return 0;
}