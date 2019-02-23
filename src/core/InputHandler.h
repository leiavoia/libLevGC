#ifndef SDL_INPUT_HANDLER_H
#define SDL_INPUT_HANDLER_H

#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
//#include "GameApplication.h"
#include "InputActions.h"

#include "Commands.h"



//predec:
// class KeyAction;
// class JoybuttonAction;


/*
TODO:
	- handle mouse input for movement and menues
	- store table of all activated joy buttons
*/


// WARNING: SDL MUST BE INITED BEFORE INPUT HANDLER IS CREATED


namespace LGC {




class InputHandler {
public:

	InputHandler( SDL_Joystick* joy=NULL );

	~InputHandler();

	void AcceptInputEvent( SDL_Event& e );

	void ProcessAllInput();

	void ChangeState( std::string str );

	void DumpToXML() {};

	keystate GetKeyState( SDLKey k );
	keystate GetButtonState( int b );

// 	static void SetPlayer( Player* p ) { Command::SetPlayer(p); }

private:

	bool Init(); // slurp up XML configs

	// these are triggers. Don't loop though all the events if there was no trigger
	bool joybutton_gotcha;
	bool keypress_gotcha;

	// maintains the current state of the joybuttons
	std::vector<bool> joybuttons;

	// maintains the state of the keyboard
	std::vector<Uint8> old_keys;
	std::vector<Uint8> new_keys;

	// ref back to the running app
	//const GameApplication& app;

	// don't do anything if the class was not inited
	bool class_was_inited;

	// pointer to the joystick if there is one. defaults to NULL
	SDL_Joystick* joystick;


	typedef std::vector<KeyAction*> 				KeyActionList;
	typedef std::vector<KeyAction*>::iterator 		KeyActionListIter;

	typedef std::vector<JoybuttonAction*> 			JoybuttonActionList;
	typedef std::vector<JoybuttonAction*>::iterator 	JoybuttonActionListIter;

	struct ActionList {
		KeyActionList 			key_actions;
		JoybuttonActionList		joybutton_actions;
		};

	// this is a map of lists of actions. The map exists so that
	// the program can switch between basic states such as PLAY and GUI
	// modes. When the program switches states, it simply loops through
	// a different set of actions.
	std::map<  std::string, ActionList  > actions;

	typedef std::map<  std::string, ActionList  >::iterator ActionsIter ;

	ActionList* current_list;



	};


// SDL KEYMAP TRANSLATION FUNCTIONS:
char 		GetKeyChar	( SDLKey& k );
bool 		IsPrintable	( SDLKey& k );
std::string 	TranslateKey	( SDLKey& k );
bool 		IsKeyCommand	( SDLKey& k );


} // end namespace LGC




#endif













