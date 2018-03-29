
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

static bool	g_continueUpdating = true;


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
	OAEmitterId defaultEmitter = audioEngine->CreateEmitter();
	if( defaultEmitter == INVALID_AUDIO_EMITTER )
	{
		audioEngine->Uninitialize();
		delete audioEngine;

		return -1;
	}

	// register a listener
	OAListenerId defaultListener = audioEngine->CreateListener();
	if( defaultListener == INVALID_AUDIO_LISTENER )
	{
		audioEngine->Uninitialize();
		delete audioEngine;

		return -1;
	}

	// create a file source (streaming)
	OASourceId fileSource		= audioEngine->CreateFileSource( "Data/RolandRhumba.wav", true );

	// create a procedural source 
	OASourceId proceduralSource = audioEngine->CreateProceduralSource( EProceduralSource::EProceduralSource_SineWave );

	// play the sources
	//audioEngine->PlaySource( defaultEmitter, fileSource );
	audioEngine->PlaySource( defaultEmitter, proceduralSource );

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