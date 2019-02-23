#include "InputHandler.h"

#include "../utilities/LogManager.h"
#include "../utilities/TinyXML/tinyxml.h"
#include "InputActions.h"


namespace LGC {




const static int MAX_JOYBUTTONS = 24;
const static std::string CONFIG_FILE = "config/inputconf.xml";





// SDL KEYMAP TRANSLATION FUNCTION:
char GetKeyChar( SDLKey& k ) {
	if ( SDL_GetModState() & KMOD_SHIFT ) {
		switch (k) {
			//SDLK_BACKSPACE		= 8
			//SDLK_TAB			= 9
			//SDLK_CLEAR		= 12
			//SDLK_RETURN		= 13
			//SDLK_PAUSE		= 19
			//SDLK_ESCAPE		= 27
			case SDLK_SPACE: return ' ';
			case SDLK_QUOTE: return '"';
			case SDLK_COMMA: return '<';
			case SDLK_MINUS: return '_';
			case SDLK_PERIOD: return '>';
			case SDLK_SLASH: return '?';
			case SDLK_SEMICOLON: return ':';
			case SDLK_EQUALS: return '+';
			case SDLK_LEFTBRACKET: return '{';
			case SDLK_BACKSLASH: return '|';
			case SDLK_RIGHTBRACKET: return '}';
			case SDLK_BACKQUOTE: return '~';
			case SDLK_0: return ')';
			case SDLK_1: return '!';
			case SDLK_2: return '@';
			case SDLK_3: return '#';
			case SDLK_4: return '$';
			case SDLK_5: return '%';
			case SDLK_6: return '^';
			case SDLK_7: return '&';
			case SDLK_8: return '*';
			case SDLK_9: return '(';
			case SDLK_a: return 'A';
			case SDLK_b: return 'B';
			case SDLK_c: return 'C';
			case SDLK_d: return 'D';
			case SDLK_e: return 'E';
			case SDLK_f: return 'F';
			case SDLK_g: return 'G';
			case SDLK_h: return 'H';
			case SDLK_i: return 'I';
			case SDLK_j: return 'J';
			case SDLK_k: return 'K';
			case SDLK_l: return 'L';
			case SDLK_m: return 'M';
			case SDLK_n: return 'N';
			case SDLK_o: return 'O';
			case SDLK_p: return 'P';
			case SDLK_q: return 'Q';
			case SDLK_r: return 'R';
			case SDLK_s: return 'S';
			case SDLK_t: return 'T';
			case SDLK_u: return 'U';
			case SDLK_v: return 'V';
			case SDLK_w: return 'W';
			case SDLK_x: return 'X';
			case SDLK_y: return 'Y';
			case SDLK_z: return 'Z';
			default: return ' ';
			}
		}

	else {
		switch (k) {
			case SDLK_SPACE: return ' ';
			case SDLK_QUOTE: return '\'';
			case SDLK_COMMA: return ',';
			case SDLK_MINUS: return '-';
			case SDLK_PERIOD: return '.';
			case SDLK_SLASH: return '/';
			case SDLK_SEMICOLON: return ';';
			case SDLK_EQUALS: return '=';
			case SDLK_LEFTBRACKET: return '[';
			case SDLK_BACKSLASH: return '\\';
			case SDLK_RIGHTBRACKET: return ']';
			case SDLK_BACKQUOTE: return '`';
			case SDLK_0: return '0';
			case SDLK_1: return '1';
			case SDLK_2: return '2';
			case SDLK_3: return '3';
			case SDLK_4: return '4';
			case SDLK_5: return '5';
			case SDLK_6: return '6';
			case SDLK_7: return '7';
			case SDLK_8: return '8';
			case SDLK_9: return '9';
			case SDLK_a: return 'a';
			case SDLK_b: return 'b';
			case SDLK_c: return 'c';
			case SDLK_d: return 'd';
			case SDLK_e: return 'e';
			case SDLK_f: return 'f';
			case SDLK_g: return 'g';
			case SDLK_h: return 'h';
			case SDLK_i: return 'i';
			case SDLK_j: return 'j';
			case SDLK_k: return 'k';
			case SDLK_l: return 'l';
			case SDLK_m: return 'm';
			case SDLK_n: return 'n';
			case SDLK_o: return 'o';
			case SDLK_p: return 'p';
			case SDLK_q: return 'q';
			case SDLK_r: return 'r';
			case SDLK_s: return 's';
			case SDLK_t: return 't';
			case SDLK_u: return 'u';
			case SDLK_v: return 'v';
			case SDLK_w: return 'w';
			case SDLK_x: return 'x';
			case SDLK_y: return 'y';
			case SDLK_z: return 'z';
			default: return ' ';
			}
		}
	}



bool IsPrintable(SDLKey& k) {
	switch (k) {
		case SDLK_SPACE: return true;
		case SDLK_QUOTE: return true;
		case SDLK_COMMA: return true;
		case SDLK_MINUS: return true;
		case SDLK_PERIOD: return true;
		case SDLK_SLASH: return true;
		case SDLK_SEMICOLON: return true;
		case SDLK_EQUALS: return true;
		case SDLK_LEFTBRACKET: return true;
		case SDLK_BACKSLASH: return true;
		case SDLK_RIGHTBRACKET: return true;
		case SDLK_BACKQUOTE: return true;
		case SDLK_0: return true;
		case SDLK_1: return true;
		case SDLK_2: return true;
		case SDLK_3: return true;
		case SDLK_4: return true;
		case SDLK_5: return true;
		case SDLK_6: return true;
		case SDLK_7: return true;
		case SDLK_8: return true;
		case SDLK_9: return true;
		case SDLK_a: return true;
		case SDLK_b: return true;
		case SDLK_c: return true;
		case SDLK_d: return true;
		case SDLK_e: return true;
		case SDLK_f: return true;
		case SDLK_g: return true;
		case SDLK_h: return true;
		case SDLK_i: return true;
		case SDLK_j: return true;
		case SDLK_k: return true;
		case SDLK_l: return true;
		case SDLK_m: return true;
		case SDLK_n: return true;
		case SDLK_o: return true;
		case SDLK_p: return true;
		case SDLK_q: return true;
		case SDLK_r: return true;
		case SDLK_s: return true;
		case SDLK_t: return true;
		case SDLK_u: return true;
		case SDLK_v: return true;
		case SDLK_w: return true;
		case SDLK_x: return true;
		case SDLK_y: return true;
		case SDLK_z: return true;
		default: return false;
		}
	}

std::string TranslateKey(SDLKey& k) {
	switch (k) {
		case SDLK_RETURN: return "enter";
		case SDLK_BACKSPACE: return "backspace";
		case SDLK_DELETE: return "delete";
		case SDLK_UP: return "up";
		case SDLK_DOWN: return "down";
		case SDLK_RIGHT: return "right";
		case SDLK_LEFT: return "left";
		case SDLK_HOME: return "home";
		case SDLK_END: return "end";
		case SDLK_PAGEUP: return "pageup";
		case SDLK_PAGEDOWN: return "pagedown";
		case SDLK_F1: return "f1";
		case SDLK_F2: return "f2";
		case SDLK_F3: return "f3";
		case SDLK_F4: return "f4";
		case SDLK_F5: return "f5";
		case SDLK_F6: return "f6";
		case SDLK_F7: return "f7";
		case SDLK_F8: return "f8";
		case SDLK_F9: return "f9";
		case SDLK_F10: return "f10";
		case SDLK_F11: return "f11";
		case SDLK_F12: return "f12";
		default: return "";
		}
	}

bool IsKeyCommand(SDLKey& k) {
	switch (k) {
		case SDLK_RETURN: return true;
		case SDLK_BACKSPACE: return true;
		case SDLK_DELETE: return true;
		case SDLK_UP: return true;
		case SDLK_DOWN: return true;
		case SDLK_RIGHT: return true;
		case SDLK_LEFT: return true;
		case SDLK_HOME: return true;
		case SDLK_END: return true;
		case SDLK_PAGEUP: return true;
		case SDLK_PAGEDOWN: return true;
		case SDLK_F1: return true;
		case SDLK_F2: return true;
		case SDLK_F3: return true;
		case SDLK_F4: return true;
		case SDLK_F5: return true;
		case SDLK_F6: return true;
		case SDLK_F7: return true;
		case SDLK_F8: return true;
		case SDLK_F9: return true;
		case SDLK_F10: return true;
		case SDLK_F11: return true;
		case SDLK_F12: return true;
		default: return false;
		}
	}





InputHandler::InputHandler( SDL_Joystick* joy ) :
	joybutton_gotcha(false),
	keypress_gotcha(false),
	joybuttons(  std::vector<bool>(MAX_JOYBUTTONS,0)  ), // 24 buttons max
	//app(app),
	class_was_inited(false),
	joystick(joy),
	actions(  std::map<  std::string, ActionList  >()  )
 	{

	for (ActionsIter i = actions.begin(); i != actions.end(); i++) {
		i->second.key_actions.clear();
		i->second.joybutton_actions.clear();
		}

	Init();
	}


InputHandler::~InputHandler() {
	// clear lists:
	for (ActionsIter i = actions.begin(); i != actions.end(); i++) {
		for (KeyActionListIter n = i->second.key_actions.begin(); n != i->second.key_actions.end(); n++) {
			delete *n;
			}
		for (JoybuttonActionListIter n = i->second.joybutton_actions.begin(); n != i->second.joybutton_actions.end(); n++) {
			delete *n;
			}
		i->second.key_actions.clear();
		i->second.joybutton_actions.clear();
		}
	}



bool InputHandler::Init() {

	// clear any current lists:
	for (ActionsIter i = actions.begin(); i != actions.end(); i++) {
		for (KeyActionListIter n = i->second.key_actions.begin(); n != i->second.key_actions.end(); n++) {
			delete *n;
			}
		for (JoybuttonActionListIter n = i->second.joybutton_actions.begin(); n != i->second.joybutton_actions.end(); n++) {
			delete *n;
			}
		i->second.key_actions.clear();
		i->second.joybutton_actions.clear();
		}

	// don't load joystick actions if we are not using one
	//joystick = (joy) ? joy : NULL;

	// set up the keys
	old_keys = std::vector<Uint8>( SDLK_LAST, 0 );
	new_keys = std::vector<Uint8>( SDLK_LAST, 0 );



	// LOAD XML FILE ---------------\/------------------------

	// load the main resource conf file:
	TiXmlDocument doc( CONFIG_FILE.c_str() );
	if ( !doc.LoadFile() ) {
		LogMgr::Write( LOGMSG_ERROR, "Could not open config file '%s'. Error='%s'.", CONFIG_FILE.c_str(), doc.ErrorDesc() );
		return false;
		}

	// check for any other loading errors
	if ( doc.Error() ) {
		LogMgr::Write( LOGMSG_ERROR, "Problem loading '%s'. Error='%s'.", CONFIG_FILE.c_str(), doc.ErrorDesc() );
		return false;
		}


	LogMgr::Write( LOGMSG_DEBUG, "Parsing input actions..." );



	// PARSE XML FILE ---------------\/------------------------

	TiXmlElement* el = NULL; // a temporary element

	// foreach GAMESTATE
	for ( TiXmlElement* xmlgamestate = doc.FirstChildElement( "GAMESTATE" ); xmlgamestate; xmlgamestate = xmlgamestate->NextSiblingElement("GAMESTATE") ) {

		// find GAMESTATE tag
		if ( !xmlgamestate ) {
			LogMgr::Write( LOGMSG_ERROR, "No GAMESTATE tag found in config file '%s'. Input configs will not load!", CONFIG_FILE.c_str());
			return false;
			}

		// add a new set of lists to the actions list:
		std::string gamestate_name( xmlgamestate->Attribute("NAME") ? xmlgamestate->Attribute("NAME") : "NO_NAME" );
		actions.insert(  std::pair< std::string, ActionList > ( gamestate_name, ActionList() )  );
		current_list = &actions[ gamestate_name ];

		// dissect each KEYACTION/JOYACTION
		for ( el = xmlgamestate->FirstChildElement(); el; el = el->NextSiblingElement() ) {

			if (!el) { break; }

			// check for action type;
			std::string value(  el->Value()  );

			if ( value == "KEYACTION") {

				// get command name
				std::string command_name( el->Attribute("COMMAND") ? el->Attribute("COMMAND") : "null" );

				// get boolmode
				std::string bm( (el->Attribute("BOOL")) ? el->Attribute("BOOL") : "/" );
				char boolmode = bm[0];

				// construct new action:
				current_list->key_actions.push_back( new KeyAction( command_name, boolmode ) );

				// add keys
				for ( TiXmlElement* xmlkey = el->FirstChildElement(); xmlkey; xmlkey = xmlkey->NextSiblingElement() ) {

					if (!xmlkey) { break; }

					// make sure it really is a key
					std::string label = xmlkey->Value();
					if ( label != "KEY" ) { continue; }


					// get key number:
					int keynum = 0;
					xmlkey->QueryIntAttribute("NUM",&keynum);
					if ( keynum < 0 ) 			{ keynum = 0; }
					if ( keynum >= SDLK_LAST ) 	{ keynum = SDLK_LAST; }

					// get keystate:
					std::string statestr( xmlkey->Attribute("STATE") ? xmlkey->Attribute("STATE") : "" );
					if ( statestr.empty() ) {
						LogMgr::Write( LOGMSG_ERROR, "No STATE attribute found in config file '%s', Row #%d. Input configs will not load!", CONFIG_FILE.c_str(), xmlkey->Row());
						return false;
						}
					keystate ks = KEYSTATE_DOWN;
					if ( statestr =="up" ) { ks = KEYSTATE_UP; }
					else if ( statestr == "held" ) { ks = KEYSTATE_HELDDOWN; }

					// add the new key
					current_list->key_actions.back()->AddKey( (SDLKey)keynum, ks );
					}
				}

			if ( value == "JOYBUTTONACTION" && joystick ) {
				// make sure joybutton number isn't larger than MAX_JOYBUTTONS;
				}

			}
		}

	std::string err_str = "Input actions loaded. ";
	err_str += "Available game states loaded from config file:";
	for (ActionsIter i = actions.begin(); i != actions.end(); i++) {
		err_str += "\n\t\"";
		err_str += i->first;
		err_str += "\"";
		}
	LogMgr::Write( LOGMSG_DEBUG, err_str.c_str() );

	class_was_inited = true;
	return true;
	}



void InputHandler::AcceptInputEvent( SDL_Event& e ) {
	switch( e.type ) {
		// if a key is down or up, we really don't
		// care what key it is since we are using GetKeys()
		case SDL_KEYDOWN: // fall through
		case SDL_KEYUP:
			keypress_gotcha = true;
			new_keys[ e.key.keysym.sym ] = e.key.state;
			break;
		case SDL_JOYBUTTONUP: // fall through
		case SDL_JOYBUTTONDOWN:
			joybutton_gotcha = true;
			joybuttons[ e.jbutton.button ] = e.jbutton.state;
			break;
// 		case SDL_JOYAXISMOTION:
// 			joybutton_gotcha = true; // FIXME: don't need this?
// 			break;
		default:
			break;
		}
	}



void InputHandler::ProcessAllInput( ) {

	if ( !class_was_inited ) { return; }

	// check all keys
//	if (keypress_gotcha) {
		for (KeyActionListIter i = current_list->key_actions.begin(); i != current_list->key_actions.end(); i++) {
			(*i)->Check(this);
			}
//		keypress_gotcha = false;
//		}

	// check all joybuttons
//	if ( joybutton_gotcha && joystick ) {
		for (JoybuttonActionListIter i = current_list->joybutton_actions.begin(); i != current_list->joybutton_actions.end(); i++) {
			(*i)->Check(this);
			}
//		joybutton_gotcha = false;
//		}


	// refresh keys and old keys
	for (unsigned int i = 0; i < (int)SDLK_LAST; i++) {
		old_keys[i] = new_keys[i];
		}



	}




void InputHandler::ChangeState( std::string str ) {
	if ( actions.find(str) == actions.end() ) {
		std::string err_str = "InputHandler::ChangeState(string): No state called \"%s\". ";
		err_str += "Available game states loaded from config file:";
		for (ActionsIter i = actions.begin(); i != actions.end(); i++) {
			err_str += "\n\t\"";
			err_str += i->first;
			err_str += "\"";
			}
		LogMgr::Write( LOGMSG_ERROR, err_str.c_str(), str.c_str() );
		LogMgr::Write( LOGMSG_ERROR, "Total states: %d", actions.size() );
		return;
		}
	current_list = &actions[ str ];
	LogMgr::Write( LOGMSG_DEBUG, "Changing input states. %d entries in state \"%s\". Total states: %d", current_list->key_actions.size(), str.c_str(), actions.size() );
	}



keystate InputHandler::GetKeyState( SDLKey k ) {
	keystate old = (keystate)old_keys[(int)k];
	keystate current = (keystate)new_keys[(int)k];
	if ( old == KEYSTATE_DOWN && current == KEYSTATE_DOWN ) { return KEYSTATE_HELDDOWN; }
	else if ( old == KEYSTATE_UP && current == KEYSTATE_UP ) { return KEYSTATE_HELDUP; }
	//LogMgr::Write( LOGMSG_DEBUG, "returning: %d", (int)current);
	return current;
	}

keystate InputHandler::GetButtonState( int b ) {
	return KEYSTATE_HELDUP;
	}



} // end namespace LGC







