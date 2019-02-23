

#include "CoreInterface.h"
#include "ResourceMgr.h"
#include <time.h>

void _init() {
	srand((unsigned int)time(NULL));
	}

void _fini() {
	LGC::Core->Cleanup();
	LGC::RM->DebugReportLoadedResources();
	}


//
// LGC::GameApplication app;
//
// int main( int argc, char* argv[] ) {
//
// 	//get args:
// 	bool fullscreen = false;
// 	bool use_sound = true;
// 	int colorDepth = 16;
// 	for (int x = 1; x < argc; x++) {
// 		if ( strcmp(argv[x],"--fullscreen") == 0 ) 		{ fullscreen = true; continue; }
// 		if ( strcmp(argv[x],"--no_sound") == 0 ) 		{ use_sound = false; continue; }
// 		if ( strcmp(argv[x], "--color_depth") == 0) 		{ colorDepth = atoi(argv[x+1]); continue; }
// 		if ( strcmp(argv[x], "--help") == 0) {
// 			std::cout
// 				<< "--fullscreen (toggles full screen option)" << std::endl
// 				<< "--color_depth [16|32] (manually sets the screen color bit depth)" << std::endl << std::flush;
// 			return 0;
// 			}
// 		}
//
//
// 	srand((unsigned int)time(NULL));
//
// 	app.Init(800,600,16,fullscreen,use_sound);
//
// 	app.RunGameLoop();
//
// 	app.Shutdown();
//
// 	LGC::Core->Shutdown();
//
// 	LGC::RM->DebugReportLoadedResources();
//
// 	//RM->DebugReportRegisteredClasses();
//
// 	return( 0 );
// 	}








