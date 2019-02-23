#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <stdlib.h>
#include <vector>
#include <deque>

#include "../core/Fonts.h"

#ifdef _LINUX
#include <sys/time.h>
#else
// #include <windows.h>
#include <SDL/SDL.h>
#endif




/**
USAGE:
Creating a timer will automatically begin the timing process.
Generally, you simply will want to call the GetTimeDelta() function
every trip through the game loop. This will update the timer's
internal data in addition to returning the delta. A delta is then
defined as the time between calls to GetTimeDelta(). To visualize,
call DrawGraph() every frame;
*/



namespace LGC {



class Timer {

public:

	Timer();

	~Timer();

	/** Returns the time difference between now and the last time you punch out */
	float GetTimeDelta();

	/** */
	void LogFPSToScreen( bool x = true ) { log_to_screen = x; }

	/** */
	void LogFPSToFile( bool x = true ) { log_to_file = x; }

	/** */
	void SetFramerateSamples( int x ) { framerate_samples = x; }

	/** */
	void DrawGraph();

	/** */
	void DrawFPS();

	/** */
	void SetDeltaSmooth( bool x ) { smooth_deltas = x;}

	/** */
	void ToggleDeltaSmooth() { smooth_deltas = !smooth_deltas; }

	/** */
	bool DeltaSmoothStatus() { return smooth_deltas; }

	/** */
	float GetFrameRate() { return FrameRate; }

  	static void ReserveResources();
	static void DumpResources();

private:

	void Log();
	void Track( float delta );
	float SmoothDelta( float x );

	bool log_to_file;
	bool log_to_screen;

	int framerate_samples;
	int FrameCount;
	float FrameRate;

	std::deque<float> most_recent;
	float recent_max;
	std::vector<long int> distrib;
	bool distrib_inited;
	long int distrib_max;
	float distrib_unit;

	float delta_avg;
	bool smooth_deltas;

#ifdef _LINUX
	struct timeval _tstart, _tend;
	struct timezone tz;
#else
	// windows uses the SDL_GetTicks() function because
	// i don't have a windows machine to test on!
	// you can comment out the existing stuff and uncomment
	// the experimental timer data if needed.

	Uint32 _tstart, _tend;

	// this is the more precise but untested windows-specific timer data:
// 	LARGE_INTEGER _tstart, _tend;
// 	LARGE_INTEGER freq;
#endif

	Font font;
	};



} // end namespace LGC



#endif



