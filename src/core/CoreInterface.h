#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include <string>
#include <SDL/SDL.h>
#include "GameState.h"
#include "Sequencer.h"
//#include "../objects/PollingObject.h"
//#include "../objects/PollingEngine.h"


// predec
class PollingObject;

// use this dandy little shortcut to save typing
#define Core CoreInterface::Instance()


namespace LGC {

class CoreInterface {
public:

	static CoreInterface* Instance() {
		static CoreInterface* x = new CoreInterface;
		return x;
		}

	// game states
	void ChangeGameState( const std::string& s );
	void ChangeGameSpace( const std::string& name );
	void PauseGame( bool x ); // true = pause, false = unpause

	// object processing
	inline void RegisterObject( PollingObject* obj, int layer=0 ) { state->RegisterObject(obj,layer); }
	void Cycle( float incoming_time );

	// audio
	void SetMasterMusicVolume( float x );
	void SetMasterFXVolume( float x );

	// global flags
	void SetGlobalFlag( const std::string& x, bool value=true );
	bool CheckGlobalFlag( const std::string& x );

	// input
	void AcceptInputEvent( SDL_Event& e );

	// config options
	// TODO

	// misc
	void CenterCameraOn( float x, float y );

	// messaging
	void SendMessage( const std::string& s );

	// this just forwards to GameApplication::Quit(). Either works.
	void Quit();

	// this needs to be called to delete the currently loaded state
	// after the game starts shutting down
	void Cleanup();

protected:
	CoreInterface();
	~CoreInterface();

	GameState* state;

	std::string state_ticket;

	};





} // end namespace LGC




#endif



