#include "CoreInterface.h"

#include "MusicTrack.h"
#include "Sound.h"
#include "../utilities/LogManager.h"
#include "GameApplication.h"


namespace LGC {




CoreInterface::CoreInterface():
	state( new GameState_Null() )
	{
	state_ticket.clear();
	}

void CoreInterface::ChangeGameState( const std::string& s ) {
	// can't switch in the middle of the game loop.
	// record change and switch on next game loop.
	state_ticket = s;
	//LogMgr::Write(LOGMSG_DEBUG, "CORE: state changed" );
	}

void CoreInterface::SendMessage( const std::string& s ) {
	state->SendMessage( s );
	}

void CoreInterface::Cycle( float incoming_time ) {
	// check for state changes here - this avoids breaking the input checking loop earlier
	if ( !state_ticket.empty() ) {
		state->ChangeGameState( state, state_ticket );
		LogMgr::Write(LOGMSG_DEBUG, "CORE: attempted state change to '%s'", state_ticket.c_str() );
		state_ticket.clear();
		}
	state->Cycle( incoming_time );
	}

void CoreInterface::PauseGame( bool x ) {
	state->PauseGame( x );
	}

void CoreInterface::ChangeGameSpace( const std::string& name ) {
	state->ChangeGameSpace( name );
	}


void CoreInterface::SetMasterMusicVolume( float x ) {
	MusicTrack::Volume(x);
	}

void CoreInterface::SetMasterFXVolume( float x ) {
	Sound::SetMasterVolume(x);
	}


void CoreInterface::SetGlobalFlag( const std::string& x, bool value ) {
	// TODO
	}

bool CoreInterface::CheckGlobalFlag( const std::string& x ) {
	// TODO
	return false;
	}


void CoreInterface::CenterCameraOn( float x, float y ) {
	state->CenterCameraOn( x, y );
	}

void CoreInterface::Quit() {
	App->Quit();
	}

void CoreInterface::AcceptInputEvent( SDL_Event& e ) {
	state->AcceptInputEvent( e );
	}

void CoreInterface::Cleanup() {
	if ( state ) { delete state; state = NULL; }
	}

CoreInterface::~CoreInterface() {
	if ( state ) { delete state; state = NULL; }
	}


} // end namespace LGC













