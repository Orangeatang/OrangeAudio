#pragma once

//////////////////////////////////////////////////////////////////////////
/// Defines
//////////////////////////////////////////////////////////////////////////

// NOTE: when changing between Windows 8.1 SDK & DirectX SDK you need to do a full rebuild; the associated libs & headers are 
// all part of the precompiled header
#define USE_DXSDK 0


//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

// xaudio
#if USE_DXSDK
    #include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\comdecl.h>
    #include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>
    #include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2fx.h>
    #include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\x3daudio.h>
    #include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xapofx.h>
#else
    #include <xaudio2.h>
    #include <xaudio2fx.h>
    #include <x3daudio.h>
    #include <xapofx.h>
#endif

// std
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <fstream>


//////////////////////////////////////////////////////////////////////////
/// Libs
//////////////////////////////////////////////////////////////////////////

#if USE_DXSDK
    // need to detect platform, not just link to the x86 version
    #pragma comment( lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\dsound.lib" )
    #pragma comment( lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\x3daudio.lib" )
#else
    #pragma comment( lib, "xaudio2.lib" )
#endif

//////////////////////////////////////////////////////////////////////////
