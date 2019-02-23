#include "GameState.h"

#include "CoreInterface.h"
// #include "GameSpace.h"
// #include "Sequencer.h"
// #include "Sound.h"
#include "../utilities/LogManager.h"
// #include "../objects/player/Player.h"
// #include "../UI/widgets/ChargeWidgets.h"
// #include "../UI/widgets/FaderMessageBox.h"
// #include "ResourceMgr.h"
// #include "../objects/CollisionObjects.h"
// #include "../fx/backgrounds/BackgroundImage.h"
// #include "../fx/transitions/Fader.h"

// #include <math.h>
// #include "extra_math.h"

//InputHandler GameState::input_handler = InputHandler(NULL);




namespace LGC {




void GameState::ChangeGameState( GameState*& active_state, const std::string& state_type ) {
	//LogMgr::Write(LOGMSG_DEBUG, "ChangeGameState() : Action is not programmed for this GameState.");
	GameState* gs = GameStateFactory::GetNew(state_type);
	if (gs) {
		delete active_state;
		active_state = gs;
		active_state->Activate();
		}
	else {
		LogMgr::Write(LOGMSG_ERROR, "GameState::ChangeGameState(): No state '%s' registered with GameStateFactory. State change order ignored.", state_type.c_str());
		}
	}
void GameState::Activate() {
     // do nothing
	}
void GameState::PauseGame( bool x ) {
	LogMgr::Write(LOGMSG_DEBUG, "PauseGame() : Action is not programmed for this GameState.");
	}
void GameState::ChangeGameSpace( const std::string& name ) {
	LogMgr::Write(LOGMSG_DEBUG, "ChangeGameSpace() : Action is not programmed for this GameState.");
	}
void GameState::RegisterObject( PollingObject* obj, int layer) {
	LogMgr::Write(LOGMSG_DEBUG, "RegisterObject() : Action is not programmed for this GameState.");
	}
void GameState::Cycle( float incoming_time ) {
	LogMgr::Write(LOGMSG_DEBUG, "Cycle() : Action is not programmed for this GameState.");
	}
void GameState::CenterCameraOn( float x, float y ) {
	LogMgr::Write(LOGMSG_DEBUG, "CenterCameraOn() : Action is not programmed for this GameState.");
	}
void GameState::AcceptInputEvent( SDL_Event& e ) {
	LogMgr::Write(LOGMSG_DEBUG, "AcceptInputEvent() : Action is not programmed for this GameState.");
	}
void GameState::SendMessage( const std::string& s ) {
	// do nothing
	}


const GameState_Null_Factory GameState_Null_Factory::auto_register_me;

} // end namespace LGC







