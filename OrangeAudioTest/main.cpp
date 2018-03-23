
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngine.h>

#include <Windows.h>

#include <thread>
#include <iostream>


//////////////////////////////////////////////////////////////////////////
/// Globals
//////////////////////////////////////////////////////////////////////////

static const OAInt64 g_defaultEmitter	= 0;
static const OAInt64 g_defaultListener	= 0;
static bool		     g_continueUpdating = true;


//////////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////////

void UpdateAudioEngine( COrangeAudioEngine* anOrangeAudioEngine );


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

    // add a .wav source
    OASourceId sourceId = audioEngine->AddSource( "Data/RolandRhumba.wav", true );
    audioEngine->PlaySound( g_defaultEmitter, sourceId );

	// create a thread to update the audio engine
	std::thread audioUpdateThread = std::thread( UpdateAudioEngine, audioEngine );

	// wait for user input before stopping
	std::cout << "Press [enter] key to exit...";
	std::getchar();

	// wait for the audio thread to stop
	g_continueUpdating = false;
	audioUpdateThread.join();

	// clean up the audio engine instance
	audioEngine->Uninitialize();
	delete audioEngine;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void UpdateAudioEngine( COrangeAudioEngine* anOrangeAudioEngine )
{
	while( g_continueUpdating )
	{
		anOrangeAudioEngine->Update( 0.0f );
		Sleep( 30 );
	}
}

//////////////////////////////////////////////////////////////////////////