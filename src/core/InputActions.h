#ifndef INPUT_ACTIONS_H
#define INPUT_ACTIONS_H

#include <vector>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>

#include "Commands.h"




namespace LGC {




// An Action is a custom key or combination of keys that execute an action.




// COMMON STRUCTURES ======================\/================================

enum keystate {
	KEYSTATE_UP=0, 		// released this frame
	KEYSTATE_DOWN, 		// pressed this frame
	KEYSTATE_HELDUP, 		// up several frames
	KEYSTATE_HELDDOWN, 		// held down
	KEYSTATE_NUM_STATES		// number of entries in this enum
	};
		
// class StateCombo {
// public:
// 	keystate target; // what the key should be to trigger
// 	keystate current; // what the key is now
// 	bool Match() { return (target == current); }
// 	bool NoMatch() { return (target != current); }
// 	StateCombo() { target = current = KEYSTATE_UP; }
// 	StateCombo( keystate ks ) { target = ks; current = KEYSTATE_UP; }
// 	};
		
		
//predec:
class InputHandler;
	
	
	
// KEYACTION  ======================\/================================
		

class KeyAction {

public:
		
	KeyAction(); 
	
	KeyAction( std::string command, char boolmode = '/' ); // "/" = N/A
		
	~KeyAction();

	void AddKey( SDLKey key, keystate state );
	
	void SetBoolMode( char mode = '/' );
	
	void SetCommand( Command* c );
	
	void Check( InputHandler* handler );
	
private:

	typedef std::multimap< SDLKey, keystate > 				KeyMap;
	typedef std::multimap< SDLKey, keystate >::iterator		KeyMapIter;
	
	KeyMap keys;
	
	Command* command;
	
	char boolmode; // "+" or "|" or "/"
	
	};
	
	
	
	
	
	
	
// JOYBUTTONACTION  ======================\/================================

class JoybuttonAction {

public:

	JoybuttonAction(); 
	
	JoybuttonAction( std::string command, char boolmode = '/' ); // "/" = N/A
		
	~JoybuttonAction();

	void AddJoyButton( int button, keystate state, SDL_Joystick* joystick );
	
	void SetBoolMode( char mode = '/' );
	
	void SetCommand( Command* c );
	
	void Check( InputHandler* handler );
	
private:

	typedef std::multimap< int, keystate > 			ButtonMap;
	typedef std::multimap< int, keystate >::iterator	ButtonMapIter;
		
	ButtonMap joybuttons;
	
	Command* command;
	
	char boolmode; // "+" or "|" or "/"
	
	};
	
	
	
	
} // end namespace LGC	




#endif







