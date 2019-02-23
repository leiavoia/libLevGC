#ifndef COMMANDS_H
#define COMMANDS_H


#include <string>






namespace LGC {





// ======== GENERIC VIRTUAL COMMAND INTERFACE =============\/================

class Command {
public:

	Command()  {};	
	
	virtual ~Command()  {};
	
	virtual void Exe() = 0;
		
	static Command* NewCommand( std::string type ); // returns heap memory. Don't forget to delete!

	};
	

	


// ===============================================================================
// ======== ALL OTHER SPECIFIC COMMANDS LISTED BELOW =============================
// ===============================================================================


class CommandNull: public Command {
public:
	CommandNull() {};	
	virtual ~CommandNull() {};
	virtual void Exe() {};	
	};
	
	
	
// class CommandPlayerMoveRight: public Command {
// public:
// 	CommandPlayerMoveRight() {};	
// 	virtual ~CommandPlayerMoveRight() {};
// 	virtual void Exe() ;	
// 	};
// 	
// class CommandPlayerMoveLeft: public Command {
// public:
// 	CommandPlayerMoveLeft() {};	
// 	virtual ~CommandPlayerMoveLeft() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandPlayerMoveUp: public Command {
// public:
// 	CommandPlayerMoveUp() {};	
// 	virtual ~CommandPlayerMoveUp() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandPlayerMoveDown: public Command {
// public:
// 	CommandPlayerMoveDown() {};	
// 	virtual ~CommandPlayerMoveDown() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandPlayerVertRest: public Command {
// public:
// 	CommandPlayerVertRest() {};	
// 	virtual ~CommandPlayerVertRest() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandPlayerHorzRest: public Command {
// public:
// 	CommandPlayerHorzRest() {};	
// 	virtual ~CommandPlayerHorzRest() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandPlayerFire: public Command {
// public:
// 	CommandPlayerFire() {};	
// 	virtual ~CommandPlayerFire() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandPlayerFireFlower: public Command {
// public:
// 	CommandPlayerFireFlower() {};	
// 	virtual ~CommandPlayerFireFlower() {};
// 	virtual void Exe();	
// 	};
// 	
// 	
// 	
// class CommandPlayerChargeWeapon: public Command {
// public:
// 	CommandPlayerChargeWeapon() {};	
// 	virtual ~CommandPlayerChargeWeapon() {};
// 	virtual void Exe();	
// 	};
// 
// class CommandChangeToActionState: public Command {
// public:
// 	CommandChangeToActionState() {};	
// 	virtual ~CommandChangeToActionState() {};
// 	virtual void Exe();	
// 	};
	
	
	
// TESTING COMMANDS ================================

// class CommandTestingLaser: public Command {
// public:
// 	CommandTestingLaser() {};	
// 	virtual ~CommandTestingLaser() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandTestingAddBall: public Command {
// public:
// 	CommandTestingAddBall() {};	
// 	virtual ~CommandTestingAddBall() {};
// 	virtual void Exe();	
// 	};
// 	
// class CommandTestingAdd10Ball: public Command {
// public:
// 	CommandTestingAdd10Ball() {};	
// 	virtual ~CommandTestingAdd10Ball() {};
// 	virtual void Exe();	
// 	};
// 
// class CommandQuit: public Command {
// public:
// 	CommandQuit() {};	
// 	virtual ~CommandQuit() {};
// 	virtual void Exe();	
// 	};
	

	
	
	
	
	
	
} // end namespace LGC	



#endif





