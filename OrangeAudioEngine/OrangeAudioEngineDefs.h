#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OAEXAudioIncludes.h>


//////////////////////////////////////////////////////////////////////////
/// Forward Declarations
//////////////////////////////////////////////////////////////////////////

class COAEAudioObject;
class IOAEAudioFile;


//////////////////////////////////////////////////////////////////////////
/// Defines
//////////////////////////////////////////////////////////////////////////

// DLL stuff
#ifdef ORANGEAUDIOENGINE_EXPORTS
    #define ORANGEAUDIOENGINE_DLL __declspec( dllexport )
#else
    #define ORANGEAUDIOENGINE_DLL __declspec( dllimport )
#endif


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

typedef std::shared_ptr<COAEAudioObject>            OAObjectPtr;
typedef std::shared_ptr<IOAEAudioFile>              OAAudioFilePtr;
typedef std::unordered_map<OAInt64, OAObjectPtr>    OAObjectMap;

//////////////////////////////////////////////////////////////////////////