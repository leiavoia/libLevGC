#include "Commands.h"
#include "../utilities/LogManager.h"


#include "CoreInterface.h"
#include "GameState.h"




namespace LGC {




// ======== COMMAND FACTORY =============\/================

Command* Command::NewCommand( std::string type ) {
// 	     if	(type == "player_move_right")		{ return new CommandPlayerMoveRight(); }
// 	else if	(type == "player_move_left")		{ return new CommandPlayerMoveLeft(); }
// 	else if	(type == "player_move_up")		{ return new CommandPlayerMoveUp(); }
// 	else if	(type == "player_move_down")		{ return new CommandPlayerMoveDown(); }
// 	else if	(type == "player_vert_rest")		{ return new CommandPlayerVertRest(); }
// 	else if	(type == "player_horz_rest")		{ return new CommandPlayerHorzRest(); }
// 	else if	(type == "player_fire") 			{ return new CommandPlayerFire(); }
// 	else if	(type == "player_fire_flower") 	{ return new CommandPlayerFireFlower(); }
// 	else if	(type == "player_charge_weapon") 	{ return new CommandPlayerChargeWeapon(); }
// 	
// 	else if	(type == "enter") 				{ return new CommandChangeToActionState(); }
// 	// FIXME ^^^ command "enter" should be embedded in a widget someday
// 	
// 	//else if	(type == "null") 				{ return new CommandNull(); }
// 	
// 	else 								{ return new CommandNull(); }

	return new CommandNull();
	
	}

 







// ===============================================================================
// ======== ALL OTHER SPECIFIC COMMANDS LISTED BELOW =============================
// ===============================================================================

// void CommandPlayerMoveRight::Exe() {
// 	if (!player) { return; }
// 	player->Control( Player::XAXIS_RIGHT );
// 	}
// 
// void CommandPlayerMoveLeft::Exe() {
// 	if (!player) { return; }
// 	player->Control( Player::XAXIS_LEFT );
// 	}
// 	
// void CommandPlayerMoveUp::Exe() {
// 	if (!player) { return; }
// 	player->Control( Player::YAXIS_UP );
// 	}
// 	
// void CommandPlayerMoveDown::Exe() {
// 	if (!player) { return; }
// 	player->Control( Player::YAXIS_DOWN );
// 	}
// 	
// void CommandPlayerVertRest::Exe() {
// 	if (!player) { return; }
// 	player->Control( Player::YAXIS_CENTER );
// 	}
// 	
// void CommandPlayerHorzRest::Exe() {
// 	if (!player) { return; }
// 	player->Control( Player::XAXIS_CENTER );
// 	}
// 	
// void CommandPlayerFire::Exe() {
// 	if (!player) { return; }
// 	player->Fire();
// 	}	
// 	
// void CommandPlayerFireFlower::Exe() {
// 	if (!player) { return; }
// 	player->FireFlower();
// 	}	
// 
// void CommandPlayerChargeWeapon::Exe() {
// 	if (!player) { return; }
// 	player->ChargeGun();
// 	}
// 
// void CommandChangeToActionState::Exe() {
// 	Core->ChangeGameState( STATE_ACTION );
// 	LogMgr::Write(LOGMSG_DEBUG, "Command: state changed" ); 
// 	}	
//



// 
// void CommandTestingLaser::Exe() {
// 	Core->RegisterObject(  new BasicLaserShot( 250, 300, 0, 2000  )  );
// 	}
// 	
// void CommandTestingAddBall::Exe() {
// 	Core->RegisterObject(  new PlayerFace( RandomNum(680)+50, RandomNum(480)+50, 100, RandomNum(360), 0  )  );			
// 	}	
// 	
// void CommandTestingAdd10Ball::Exe() {
// 	for (int n=0; n < 10; n++) {
// 		Core->RegisterObject(  new PlayerFace( RandomNum(680)+50, RandomNum(480)+50, 100, RandomNum(360), 0  )  );
// 		}
// 	}
// 	
// void CommandQuit::Exe() {
// 	LogMgr::Write(LOGMSG_DEBUG, "CommandQuit: not wired to anything yet :-(");
// 	exit(0);
// 	}

	
	
	
} // end namespace LGC	





