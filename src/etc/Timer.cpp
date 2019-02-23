#include "Timer.h"
#include <iostream>
#include <float.h>
#include <GL/gl.h>
#include "../core/Fonts.h"
#include "../core/ResourceMgr.h"
#include <sstream>





namespace LGC {



static const unsigned int GRAPH_SIZE = 500;
static const unsigned int DELTA_SMOOTH_SAMPLE = 5;
static const float MAX_DELTA = 1.0 / 20.0;


Timer::Timer() {
	log_to_file = false;
	log_to_screen = false;
	framerate_samples = 100;
	FrameCount = 0;
	FrameRate = 0;
	delta_avg = 0;
	smooth_deltas = false;

	// init the record keepers:
	most_recent = std::deque<float>();
	distrib = std::vector<long int>(GRAPH_SIZE,0);
	distrib_inited = false;
	distrib_max = 1;
	recent_max = 0;

	// skip the normal resource acquisition interface
	font = RM->GetFont("FPScounter","Timer");


	// Get the clock started:

#ifdef _LINUX
	gettimeofday(&_tstart, &tz);
#else
	_tstart = SDL_GetTicks();
	// EXPERIMENTAL WINDOWS-SPECIFIC (untested):
// 	QueryPerformanceFrequency(&freq);
// 	QueryPerformanceCounter(&_tstart);
#endif
	}


Timer::~Timer() {
	RM->DumpFont(font, "Timer");
	}

float Timer::GetTimeDelta() {

	static double fps_delta = 0;
	static double fps_last = 0;

#ifdef _LINUX
	gettimeofday(&_tend,&tz);
	double t1 = (double)_tstart.tv_sec + (double)_tstart.tv_usec / 1000000;
	double t2 = (double)_tend.tv_sec + (double)_tend.tv_usec / 1000000;
#else
	_tend = SDL_GetTicks();
	double t1 = double(_tstart) / 1000.0;
 	double t2 = double(_tend) / 1000.0;
 	// EXPERIMENTAL WINDOWS-SPECIFIC (untested):
// 	QueryPerformanceCounter(&_tend);
// 	double t1 = (double)_tstart.QuadPart / (double)freq.QuadPart;
// 	double t2 = (double)_tend.QuadPart / (double)freq.QuadPart;
#endif

	float delta = float(t2-t1);

	// calculate FPS
	if (++FrameCount >= framerate_samples) {
		fps_delta = t2 - fps_last ;
		FrameRate = double(framerate_samples) / fps_delta ;
		FrameCount = 0;
		if (fps_last != 0) { Log(); }
		fps_last = t2;
   		}

	_tstart = _tend;

	// average the delta out if requested
	float new_delta = (smooth_deltas) ? SmoothDelta(delta) : delta;

	// hiccup protection:
	if (!smooth_deltas) {
		if ( new_delta > MAX_DELTA ) {
//			std::cout << "Detected time delta hiccup" << std::endl;
			new_delta = MAX_DELTA;
			}
		}

	Track( float(delta) );
	return new_delta;
	}


float Timer::SmoothDelta( float x ) {

	float total = 0;
	float samples = 0;
	if (most_recent.size() <= DELTA_SMOOTH_SAMPLE+3) { return x; }
	for ( unsigned int i = most_recent.size()-1; i >= most_recent.size()-1-DELTA_SMOOTH_SAMPLE ; i--) {
		total += most_recent[i];
		samples += 1.0;
		}

	delta_avg = total / samples;

	return ( delta_avg * (float)DELTA_SMOOTH_SAMPLE   +   x ) / ((float)DELTA_SMOOTH_SAMPLE + 1);

	}


void Timer::Log() {
	 if (log_to_screen) {
      	std::cout << "FPS: " << FrameRate << std::endl;
		}
	 if (log_to_file) {
      	// TODO
		}
	 }



void Timer::Track( float delta ) {

	static bool first_trip = true;
	static unsigned int hits = 0;

	if (first_trip) { first_trip = false; return; }

	// log most recent
	if ( most_recent.size() == GRAPH_SIZE) { most_recent.pop_front(); }
	most_recent.push_back( delta );
	if (recent_max == 0) { recent_max = delta; }
	else if ( delta > recent_max ) {
		if ( delta < recent_max * 4 ) { recent_max = delta;  }
		}

	// set up the distribution graph if needed
	if ( !distrib_inited  &&  ++hits >= GRAPH_SIZE ) {

		// first find the high and low points + average
		float high = 0, low = FLT_MAX;
		for ( std::deque<float>::iterator i = most_recent.begin(); i != most_recent.end(); i++ ) {
			if ( *i < low  ) { low = *i;  }
			if ( *i > high ) { high = *i; }
			}

		// setup distrib vector
		//distrib_unit = (high - low) / float(GRAPH_SIZE);
		distrib_unit = (  (1.0f/10.0f) - (1.0f/250.0f)  ) / float(GRAPH_SIZE);
		distrib_inited = true;

		}

	else if (distrib_inited) {
		// log the distribution
		unsigned long int index = int( delta / distrib_unit );
		if (index >= GRAPH_SIZE) { index = GRAPH_SIZE - 1; }
		else if (index < 0) { index = 0; }
		distrib[index]++;
		if ( distrib[index] > distrib_max ) { distrib_max = distrib[index]; }
		}
	}


void Timer::DrawGraph() {

	glBindTexture( GL_TEXTURE_2D, 0 );

	// draw base quad:
	if (smooth_deltas) {
		glColor4f(0.1,0.1,0.3,0.5);
		}
	else {
		glColor4f(0,0,0,0.5);
		}
	glBegin(GL_QUADS);
		glVertex2f( 0, 50 );
		glVertex2f( GRAPH_SIZE, 50 );
		glVertex2f( GRAPH_SIZE, 0 );
		glVertex2f( 0, 0 );
	glEnd();

	// draw most recent graph
	glBegin(GL_LINE_STRIP);
		glColor4f(0.8, 0.6, 0.6, 0.5);
		glVertex2f( GRAPH_SIZE, 50 ); // top of bar
		glVertex2f( 0, 50 ); // top of bar
		glColor4f(1.0, 0.8, 0.8, 1.0);
		for ( unsigned int i = 0; i < most_recent.size(); i++ ) {
			glVertex2f( i, (1.0 - (most_recent[i] / recent_max)) * 50); // top of bar
			}
	glEnd();

	// draw distrib graph
	if (distrib_inited) {
		glBegin(GL_LINES);
			glColor4f(1.0, 1.0, 0.5, 1.0);
			for ( unsigned int i = 0; i < distrib.size(); i++ ) {
				glVertex2f( i, 100 ); // bottom of bar
				glVertex2f( i, (1.0 - ((float)distrib[i] / (float)distrib_max)) * 50 + 50); // top of bar
				}
		glEnd();
		}

	}

void Timer::DrawFPS() {
	std::stringstream ss;
	ss << (int)FrameRate;
	glColor4f(1,1,1,1);
	font.RenderFontToScreen( ss.str(), 5, 0 );
	}


} // end namespace LGC


