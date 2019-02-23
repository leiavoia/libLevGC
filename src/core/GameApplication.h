#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../etc/Timer.h"
#include "RenderingLayer.h"




namespace LGC {



/*

TODO:
- pathname of executable for relative files

*/






#define App GameApplication::GetInstance()

class GameApplication {
public:

	static GameApplication* GetInstance();

	~GameApplication();

	void Init(
		int width=800,
		int height=600,
		int bitdepth=32,
		bool fullscreen=false,
		bool sound = true,
		std::string app_path = "./"
		);

	void RunGameLoop();
	void Quit(); // stops the game loop. call from within the game itself
	void Shutdown(); // cleans up and terminates app

	/* function to reset our viewport after a window resize */
	void ResizeWindow( int width, int height );

	void ToggleFullScreen();

	inline const int ScreenWidth() { return Renderer->ScreenWidth(); };
	inline const int ScreenHeight() { return Renderer->ScreenHeight(); };
	inline const int ColorBitDepth() { return Renderer->ColorBitDepth(); };

	inline std::string GetAppDirectoryPath() { return application_path; }

	inline void SetPolling( bool x ) { do_polling = x; }
	inline void SetCollisions( bool x ) { do_collisions = x; }
	inline void SetDrawing( bool x ) { do_drawing = x; }
	inline void SetCollisionDrawing( bool x ) { do_collision_drawing = x; }
	inline void SetDrawFPS( bool x ) { do_fps = x; }
	inline void SetDrawFPSGraph( bool x ) { do_fps_graph = x; }

	void SetWindowTitle( const char* title );
	void SetIcon( std::string path );

private:

	GameApplication();

	std::string application_path;

	Timer* timer;

	int InitGL();
	void InitAudio();
	void handleKeyPress( SDL_KeyboardEvent *key );

	bool game_loop_done;

	// testing hacks
	//Player* player;

	bool do_polling;
	bool do_collisions;
	bool do_drawing;
	bool do_collision_drawing;
	bool do_fps;
	bool do_fps_graph;
	bool inited;

	};





} // end namespace LGC




#endif




