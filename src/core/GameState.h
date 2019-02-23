#ifndef GAME_STATE_H
#define GAME_STATE_H


#include <string>
#include <SDL/SDL.h>
#include "Sequencer.h"
#include "../etc/PluggableFactory.h"
// #include "Fonts.h"
// #include "Texture.h"
// #include "InputHandler.h"



//predec
// class GameSpace;
// class Player;
// class GunChargeWidget;
// class AirChargeWidget;
// class HealthChargeWidget;
// class AmmoChargeWidget;
class PollingObject;
//class FadeOutTransition;
//
// enum State {
// 	STATE_TITLE,
// 	STATE_ACTION,
// 	STATE_SUBMENU,
// 	STATE_POPUP,
// 	STATE_CINEMA,
// 	STATE_ENDING,
// 	STATE_PAUSE,
// 	STATE_NULL
// 	};



namespace LGC {


// -----------------------------------------------------------


class GameState {
public:

	GameState() {};
	virtual ~GameState() {};

	// game states
	// note: gamestate needs a reference to the active state, even though it *IS* the active state
	// - this is because the core interface needs to keep tabs on it.
	void ChangeGameState( GameState*& active_state, const std::string& state_type );
	virtual void Activate();
	virtual void PauseGame( bool x );

	// game space
	virtual void ChangeGameSpace( const std::string& name );

	// object processing
	virtual void RegisterObject( PollingObject* obj, int layer=0);
	virtual void Cycle( float incoming_time );

	// input
	virtual void AcceptInputEvent( SDL_Event& e );

	// misc
	virtual void CenterCameraOn( float x, float y );

	virtual void SendMessage( const std::string& s );

protected:

	//friend class FadeOutTransition;

	//static InputHandler input_handler;
	};



// -----------------------------------------------------------


// class GameStateFactory : public PluggableFactory<GameState> {
// 	public:
// 		~GameStateFactory() {}
// 	protected:
// 		GameStateFactory( const std::string& s ) : PluggableFactory<GameState>(s) {}
// 	};

typedef PluggableFactory<GameState> GameStateFactory;

// -----------------------------------------------------------



// this is basically just a dummy stub
class GameState_Null: public GameState {
public:
	GameState_Null() {}
	~GameState_Null() {}

	//void ChangeGameState( GameState*& active_state, const std::string& state_type );
	};


// null gamestate factory:
class GameState_Null_Factory : public GameStateFactory {
	public:
		~GameState_Null_Factory() {}
	protected:
		GameState_Null_Factory() : GameStateFactory("GameState_Null") {}
		inline GameState* CreateNew() { return new GameState_Null(); }
		static const GameState_Null_Factory auto_register_me;
	};



// -----------------------------------------------------------
//
//
//
//
// class GameState_Action: public GameState {
// public:
//
// 	GameState_Action();
// 	~GameState_Action();
//
// 	void ChangeGameState( GameState*& active_state, State state_type );
// 	void Activate();
// 	void PauseGame();
// 	void ChangeGameSpace( const std::string& name );
// 	void RegisterObject( PollingObject* obj, Layer layer = MID_SPRITE_LAYER);
// 	void Cycle( float incoming_time );
// 	void CenterCameraOn( float x, float y );
// 	void AcceptInputEvent( SDL_Event& e );
// private:
//
// 	Sequencer* sequencer;
// 	GameSpace* gamespace;
//
// 	Player* 			player;
// 	GunChargeWidget* 	w1;
// 	AirChargeWidget* 	w2;
// 	HealthChargeWidget* w3;
// 	AmmoChargeWidget* 	w4;
//
// 	Font f;
//
// 	int last_mouse_x;
// 	int last_mouse_y;
// 	bool left_mouse_pressed_last_frame;
// 	bool right_mouse_pressed_last_frame;
//
// 	};
//
//
//
//
// // -----------------------------------------------------------
//
//
//
//
// class GameState_Title: public GameState {
// public:
//
// 	GameState_Title();
// 	~GameState_Title();
//
// 	void ChangeGameState( GameState*& active_state, State state_type );
// 	void Activate();
// 	void PauseGame();
// 	void RegisterObject( PollingObject* obj, Layer layer = MID_SPRITE_LAYER);
// 	void Cycle( float incoming_time );
// 	void AcceptInputEvent( SDL_Event& e );
// 	void InternalChangeGameState( State s );
// private:
//
// 	Sequencer* sequencer;
//
// 	Texture bg;
//
// 	};
//
//
//
//
// // -----------------------------------------------------------
//



} // end namespace LGC






#endif




