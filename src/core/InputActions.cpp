
#include "InputActions.h"
#include "../utilities/LogManager.h"
#include "InputHandler.h"
#include <algorithm>




namespace LGC {




// =================================================================
// ========= KeyAction =============================================
// =================================================================



KeyAction::KeyAction() : 
	keys(KeyMap()),
	command( Command::NewCommand("null") ),
	boolmode('/')
	{
	keys.clear();
	}

KeyAction::KeyAction( std::string command, char boolmode ) : 
	keys(KeyMap()),
	command( Command::NewCommand( command ) )
	{
	SetBoolMode(boolmode);
	}
	
KeyAction::~KeyAction() {
	delete command;
	}

void KeyAction::AddKey( SDLKey key, keystate state ) {
	keys.insert(  std::make_pair( key, state )  );
	}

void KeyAction::SetBoolMode( char mode ) {
	boolmode = mode;
	if (
		boolmode != '+' &&
		boolmode != '|'  
		) {
		boolmode = '/';
		}
	}

void KeyAction::SetCommand( Command* c ) { 
	if (!c) { command = Command::NewCommand("null"); }
	else { command = c; }
	}


void KeyAction::Check( InputHandler* handler ) {

	// check keyboard
	for (KeyMapIter i = keys.begin(); i != keys.end(); i++) {
	//LogMgr::Write( LOGMSG_DEBUG, "Target: %d Now: %d", (int)i->second, (int)handler->GetKeyState(i->first)  );
		// stop on false if boolmode requires all keys to be involved
		if ( boolmode == '+' && i->second != handler->GetKeyState(i->first) ) { return; }
		// do command if boolmode only needs one key hit
		else if ( (boolmode == '|' || boolmode == '/') && i->second == handler->GetKeyState(i->first) ) { 
			command->Exe();
			return; 
			}					
		}	

	// if we got this far, the "+" mode hasn't failed yet, so execute command
	if ( !keys.empty() && boolmode == '+' ) { command->Exe(); }
		
	
	}


	
	
	
	
	
// =================================================================
// ========= JoybuttonAction =======================================
// =================================================================



JoybuttonAction::JoybuttonAction() : 
	joybuttons(ButtonMap()),
	command( Command::NewCommand("null") ),
	boolmode('/')
	{
	}

JoybuttonAction::JoybuttonAction( std::string command, char boolmode ) : 
	joybuttons(ButtonMap()),
	command( Command::NewCommand( command ) )
	{
	SetBoolMode(boolmode);
	}
	
JoybuttonAction::~JoybuttonAction() {
	delete command;
	}

void JoybuttonAction::AddJoyButton( int button, keystate state, SDL_Joystick* joystick ) {
	// OOB?
	if (button > SDL_JoystickNumButtons(joystick) -1) { button = SDL_JoystickNumButtons(joystick) -1; } 
	if (button < 0) { button = 0; } 
	joybuttons.insert(  std::pair<int,keystate>(button,state)  );
	}

void JoybuttonAction::SetBoolMode( char mode ) {
	boolmode = mode;
	if (
		boolmode != '+' && 
		boolmode != '|'  
		) {
		boolmode = '/';
		}
	}

void JoybuttonAction::SetCommand( Command* c ) { 
	if (!c) { command = Command::NewCommand("null"); }
	else { command = c; }
	}


void JoybuttonAction::Check( InputHandler* handler ) {
	
	// check joystick
	for (ButtonMapIter i = joybuttons.begin(); i != joybuttons.end(); i++) {
		// stop on false if boolmode requires all keys to be involved
		if ( boolmode == '+' && i->second != handler->GetButtonState(i->first) ) { return; }
		// do command if boolmode only needs one key hit
		if ( (boolmode == '|' || boolmode == '/') && i->second == handler->GetButtonState(i->first) ) { 
			command->Exe();
			return; 
			}					
		}
		
	// if we got this far, the "+" mode hasn't failed yet, so execute command
	if ( !joybuttons.empty() && boolmode == '+' ) { command->Exe(); }
		
	}


	
} // end namespace LGC	













