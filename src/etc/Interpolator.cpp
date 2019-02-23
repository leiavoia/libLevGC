#include "Interpolator.h"
#include <math.h>




namespace LGC {



Interpolator::Interpolator() : repeat(TRIANGLE), maxtime(0) {

	}
	
Interpolator::Interpolator( Repeat r ) : repeat(r), maxtime(0) {

	}
	
	
void Interpolator::AddPoint(float time, float value) {
	points[time] = value;
	if (time > maxtime) { maxtime = time; }
	}
	
void Interpolator::Clear() {
	points.clear();
	maxtime = 0;
	}
	
	
float Interpolator::Value( float time ) {

	static int modulo = 0;
	
	// 1-point graphs are easy ;-)
	if ( points.size() == 1 ) { return points.begin()->second; }
	else if ( points.size() == 0 ) { return 0; }
	
	// normalize the time to fit inside the graph
	if ( repeat != NO_REPEAT && time > maxtime) {
		modulo = int(time / maxtime) % 2;	
		time -= maxtime * floor(time / maxtime);		
		}
	if ( time < 0 ) { time = 0; }
	
	// reverse time if doing TRIANGLE
	if (repeat==TRIANGLE && modulo==1) {
		time = maxtime - time;
		}

	// Find our spot 			
	for( point_iter i = points.begin(); i != points.end(); i++ ) {
		point_iter next = i;
		next++;			
		// off the end?
		if (next == points.end() && repeat==NO_REPEAT) { return i->second; }		
		if( i->first <= time && next->first >= time ) {			
			float ratio = ( time - i->first ) / (next->first - i->first); // ratio of difference
			return i->second + ratio * ( next->second - i->second );
			}
		}		
	
	return 0;
		
	}


void Interpolator::Preset_Gradient	( float time, float start, float end ) {
	AddPoint(0.0, start);
	AddPoint(time, end);
	repeat = NO_REPEAT;
	}
	
void Interpolator::Preset_V ( float time, float end, float mid ) {
	AddPoint(0.0, end);
	AddPoint(time*0.5, mid);
	AddPoint(time, end);
	repeat = TRIANGLE;
	}
		
void Interpolator::Preset_Alternate ( float time, float on, float off ) {
	AddPoint(0.0, on);
	AddPoint(time * 0.49999, on);
	AddPoint(time * 0.5, off);
	AddPoint(time, off);
	repeat = SAWTOOTH;
	}
	
void Interpolator::Preset_BlinkFast ( float time, float on, float off ) {
	AddPoint( 0, off );	
	AddPoint( time * 0.93, off );
	AddPoint( time, on );
	repeat = TRIANGLE;
	}
	
void Interpolator::Preset_BlinkSlow ( float time, float on, float off ) {
	AddPoint( 0, off );	
	AddPoint( time * 0.75, off );
	AddPoint( time, on );
	repeat = TRIANGLE;
	}
	

	
} // end namespace LGC	


	


