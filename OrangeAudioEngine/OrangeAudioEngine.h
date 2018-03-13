#pragma once

//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include <OrangeAudioEngineDefs.h>


//////////////////////////////////////////////////////////////////////////
/// Forward Declarations
//////////////////////////////////////////////////////////////////////////

struct  IXAudio2;
struct  IXAudio2MasteringVoice;

class   COAEEmitterObject;
class   COAESourceManager;
class   COAEVoiceManager;


//////////////////////////////////////////////////////////////////////////
/// COrangeAudioEngine
//////////////////////////////////////////////////////////////////////////

// the base class that manages all voices and audio graphs
class ORANGEAUDIOENGINE_DLL COrangeAudioEngine
{
public :

    //////////////////////////////////////////////////////////////////////////

    COrangeAudioEngine();
    ~COrangeAudioEngine();

    //////////////////////////////////////////////////////////////////////////

    bool			Initialize();
    void			Uninitialize();

    //////////////////////////////////////////////////////////////////////////

    bool			RegisterEmitter( const OAEmitterId& anId );
    void			UnregisterEmitter( const OAEmitterId& anId );

    bool			RegisterListener( const OAListenerId& anId );
    void			UnregisterListener( const OAListenerId& anId );

    //////////////////////////////////////////////////////////////////////////

    OASourceId      AddSource( const std::string& aFileName );
    void            RemoveSource( const OASourceId& aSourceId );

	//////////////////////////////////////////////////////////////////////////

	OAVoiceId   	PlaySound( const OAEmitterId& anEmitterId, const std::string& anAudioFile );
    OAVoiceId       PlaySound( const OAEmitterId& anEmitterId, const OASourceId& aSourceId );



private:

    //////////////////////////////////////////////////////////////////////////

    typedef std::unordered_map<OAEmitterId, OAEmitterPtr>    OAEmitterMap;
    typedef std::unordered_map<OAListenerId, OAEmitterPtr>   OAListenerMap;

	//////////////////////////////////////////////////////////////////////////

	OAEmitterPtr	GetEmitter( const OAEmitterId& anEmitterId );

    //////////////////////////////////////////////////////////////////////////

    bool            InitializeXAudio2();
    bool            InitializeManagers();
    void            Cleanup();

    //////////////////////////////////////////////////////////////////////////

    IXAudio2*                           m_xaudioInterface;
    IXAudio2MasteringVoice*             m_xaudioMasteringVoice;
	X3DAUDIO_HANDLE			            m_x3DInstance;

    COAESourceManager*                  m_sourceManager;
    COAEVoiceManager*                   m_voiceManager;

    bool                                m_initialized;

    // disable warnings about stl containers being inaccessible outside the DLL, these will only be used internally 
#pragma warning( push )
#pragma warning( disable: 4251 )
    OAEmitterMap                        m_emitters;
    OAListenerMap                       m_listeners;
#pragma warning( pop )
};

//////////////////////////////////////////////////////////////////////////
