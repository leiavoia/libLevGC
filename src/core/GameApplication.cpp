#include "GameApplication.h"

#include <SDL/SDL.h>
#include <iostream>


#include "../etc/Timer.h"
#include "CoreInterface.h"
#include "ResourceMgr.h"
#include "../utilities/LogManager.h"
#include "InputHandler.h"
#include "GameSpace.h"




// KILLME
// void handleKeyPress( SDL_KeyboardEvent *key );


namespace LGC {




inline int RandomNum(int max) {
	return int(   max * (double)rand() / (double)RAND_MAX   );
	}




GameApplication* GameApplication::GetInstance() {
	static GameApplication* app = new GameApplication();
	return app;
	}


GameApplication::GameApplication() :
	application_path("./"),
	game_loop_done(true),
	do_polling(true),
	do_collisions(true),
	do_drawing(true),
	do_collision_drawing(false),
	do_fps(false),
	do_fps_graph(false),
	inited(false)
	{

	}

GameApplication::~GameApplication() {

	}

void GameApplication::Init( int width, int height, int bitdepth, bool fullscreen, bool sound, std::string app_path ) {

	if (inited) { return; }

	application_path = app_path;

	// init video
	Renderer->Init( width, height, bitdepth, fullscreen );

	// let GameSpace know what size we are
	GameSpace::SetScreenSize( width, height );


	// open sound

	// 	int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize)
	//
	// 	frequency
	// 		Output sampling frequency in samples per second (Hz).
	// 		you might use MIX_DEFAULT_FREQUENCY(22050) since that is a good value for most games.
	// 	format
	// 		Output sample format.
	// 	channels
	// 		Number of sound channels in output.
	// 		Set to 2 for stereo, 1 for mono. This has nothing to do with mixing channels.
	// 	chunksize
	// 		Bytes used per output sample.

	if (sound) {
		InitAudio();
		}
	else {
		RM->DisableAudio();
		LogMgr::Write(LOGMSG_DEBUG, "Audio Disabled" );
		}

// 	SDL_EnableKeyRepeat( 50, 50 );

	inited = true;
	}


void GameApplication::RunGameLoop() {

	LogMgr::Write(LOGMSG_DEBUG, "game loop started");

	timer = new Timer();

	game_loop_done = false;
	SDL_Event event;

	// clear events list:
	while ( SDL_PollEvent( &event ) ) { /* DO NOTHING */ }

	// run loop
	while ( !game_loop_done ) {

		/* handle the events in the queue */
		while ( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
// 				case SDL_ACTIVEEVENT:
// 					/* Something's happend with our focus
// 					* If we lost focus or we are iconified, we
// 					* shouldn't draw the screen
// 					*/
// 					if ( event.active.gain == 0 )
// 						isActive = false;
// 					else
// 						isActive = true;
// 					break;
				case SDL_VIDEORESIZE:
					Renderer->ResizeWindow( event.resize.w, event.resize.h );
					break;
				case SDL_MOUSEMOTION: // fall through
				case SDL_MOUSEBUTTONUP: // fall through
				case SDL_MOUSEBUTTONDOWN:
					Core->AcceptInputEvent(event);
					break;
				case SDL_KEYUP: // TODO: fall through
					//input_handler.AcceptInputEvent(event);
					Core->AcceptInputEvent(event);
					break;
				case SDL_KEYDOWN:
					//input_handler.AcceptInputEvent(event);
					Core->AcceptInputEvent(event);
					// [!]  enables F1/fullscreen and ESC/quit
					handleKeyPress( &event.key );
					break;
				case SDL_JOYBUTTONUP: // fall through
				case SDL_JOYBUTTONDOWN:
					// TODO
					break;
				case SDL_QUIT:
					/* handle quit requests */
					game_loop_done = true;
					break;
				default:
					break;
				}
			}

		float dt = timer->GetTimeDelta();

		Core->Cycle(dt);

		if ( do_fps ) { timer->DrawFPS(); }

		if ( do_fps_graph ) { timer->DrawGraph(); }

  		Renderer->Render();

// 		std::cout << timer->GetFrameRate() << " fps\n";

		} // END GAME LOOP


	// SHUTDOWN PROCEDURES -----------\/-----------------

	delete timer;

	Core->Cleanup(); // deletes the currently loaded state

	// return control back to app caller...
	}




void GameApplication::ToggleFullScreen() {
	Renderer->ToggleFullScreen();
	}


/** function to release/destroy our resources and restoring the old desktop */
void GameApplication::Shutdown() {
	if (!inited) { return; }
	game_loop_done = true;
	Mix_CloseAudio();
	Renderer->Shutdown();
	inited = false;
	}


/* function to reset our viewport after a window resize */
void GameApplication::ResizeWindow( int width, int height ) {
	Renderer->ResizeWindow( width, height );
	}



void GameApplication::InitAudio() {

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) == -1) { // Really bad!
		LogMgr::Write(LOGMSG_ERROR, "Could not initalize SDL audio subsystem: ", SDL_GetError() );
		return;
	}

	// Open 22.05KHz, signed 16bit, system byte order, stereo audio, using 1024 byte chunks
	// MIX_DEFAULT_FREQUENCY
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
		LogMgr::Write(LOGMSG_ERROR, "Unable To Open Audio: %s", SDL_GetError( ));
		}

	Mix_AllocateChannels(24);

	int audio_rate, audio_channels;
	Uint16 audio_format;
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	LogMgr::Write(LOGMSG_DEBUG, "Opened audio at %d Hz %d bit %s",
	 	audio_rate,
		(audio_format&0xFF),
		(audio_channels > 1) ? "stereo" : "mono"
		);
	}



/* function to handle key press events */
//
void GameApplication::handleKeyPress( SDL_KeyboardEvent *key ) {
	switch ( key->keysym.sym ) {
		case SDLK_F12:
			/* ESC key was pressed */
			game_loop_done = true;
			break;
		case SDLK_F1:
			/* F1 key was pressed
			* this toggles fullscreen mode
			*/
			Renderer->ToggleFullScreen();
			break;
		default: ;;
		}
 	}


void GameApplication::SetWindowTitle( const char* title ) {
	SDL_WM_SetCaption( title, title ); // window title, icon title
	}

void GameApplication::SetIcon( std::string path ) {
	SDL_Surface* s = IMG_Load( path.c_str() );
	SDL_WM_SetIcon( s, NULL );
	}

void GameApplication::Quit() { game_loop_done = true; }



// const std::string GetAppDirectoryPath() {
//
// 	return application_path;
// // #ifdef _WIN32
// // // 	THIS CODE IS FOR WINDOWS. IT LOCATES THE PARENT DIR OF THE PATH THE EXE IS IN.
// // // 	Find parent of executable directory
// // 	TCHAR szPath[MAX_PATH];
// // 	std::string execPath;
// // 	std::string rootPath;
// // 	if(GetModuleFileName(hInstance, szPath, MAX_PATH)) {
// // 		std::string path(szPath);
// // 		size_t last = path.rfind('\\');
// // 		if(last != std::string::npos) {
// // 			execPath = path.substr(0, last+1);
// // 			last = path.rfind('\\', last-1);
// // 			if(last != std::string::npos)
// // 				rootPath = path.substr(0, last+1);
// // 			}
// // 		}
// // 	if(rootPath.empty()) {
// // 		MessageBox( NULL, "Could not locate root path from executable path", "Fatal error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
// // 		exit(0);
// // 		}
// // 	return rootPath;
// // #else
// // 	return "./";
// // #endif
//
// 	}
//
//
// const std::string GetConfigFileDirectoryPath() {
// #ifdef _WIN32
// 	return "./";
// #else
// 	return "./";
// #endif
// 	}

} // end namespace LGC


