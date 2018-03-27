#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OAEAudioIncludes.h>


//////////////////////////////////////////////////////////////////////////
/// Forward Declarations
//////////////////////////////////////////////////////////////////////////

class COAEEmitterObject;
class COAEVoice;
class IOAESource;


//////////////////////////////////////////////////////////////////////////
/// Defines
//////////////////////////////////////////////////////////////////////////

// DLL stuff
#ifdef ORANGEAUDIOENGINE_EXPORTS
    #define ORANGEAUDIOENGINE_DLL __declspec( dllexport )
#else
    #define ORANGEAUDIOENGINE_DLL __declspec( dllimport )
#endif

#define INVALID_AUDIO_SOURCE    0
#define INVALID_AUDIO_EMITTER   0
#define INVALID_AUDIO_LISTENER  0
#define INVALID_AUDIO_VOICE     0


//////////////////////////////////////////////////////////////////////////
/// Typedefs
//////////////////////////////////////////////////////////////////////////

typedef unsigned char                               OAUInt8;
typedef unsigned short                              OAUInt16;
typedef unsigned long                               OAUInt32;
typedef unsigned __int64                            OAUInt64;

typedef char                                        OAInt8;
typedef short                                       OAInt16;
typedef long                                        OAInt32;
typedef __int64                                     OAInt64;

typedef float                                       OAFloat32;
typedef double                                      OAFloat64;

//////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<COAEEmitterObject>          OAEmitterPtr;
typedef std::shared_ptr<COAEVoice>                  OAVoicePtr;
typedef std::shared_ptr<IOAESource>                 OASourcePtr;

//////////////////////////////////////////////////////////////////////////

typedef OAUInt64                                    OAEmitterId; 
typedef OAUInt64                                    OAListenerId;
typedef OAUInt64                                    OASourceId;
typedef OAInt32                                     OAVoiceId;


//////////////////////////////////////////////////////////////////////////
/// Enumerations
//////////////////////////////////////////////////////////////////////////

enum class ESourceType : OAUInt8
{
	ESourceType_Memory,			// loaded in to memory
	ESourceType_Streaming,		// streamed from disk
	ESourceType_Procedural		// procedurally generated
};

//////////////////////////////////////////////////////////////////////////

enum class EProceduralSource : OAUInt8
{
	EProceduralSource_SineWave
};

//////////////////////////////////////////////////////////////////////////