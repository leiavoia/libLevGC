#ifndef Interpolator_H
#define Interpolator_H

#include <map>

/*
DIRECTIONS:
This class is a linear interpolator. Given a series of 
points in time to use as a graph, it will return a Value() 
at a given time. Internally, it graphs time on one axis and 
values on the other. Note that "time" need not actually be time,
but can be any unit of measure. You could substitute "time" with
the word "length" if that makes it easier for you.


EXAMPLE: Create a simple gradient from zero to one in 1-second intervals

Interpolator i = Interpolator( Interpolator::TRIANGLE );
i.AddPoint(0,0);
i.AddPoint(1,1);

// if you wanted to know what the value was at 3/10ths of a second:
i.Value(0.3);


REPETITION:

given the pattern:
*
 *
  *

NO_REPEAT will clip the ends of the interpolation time. If you go past
the end it will return the end time.
*
 *
  **********
  
SAWTOOTH will repeat the graph as-is in order. If you go past
the end, it will simply repeat like so:
*  *  *  *
 *  *  *  *
  *  *  *  *
  
TRIANGLE will repeat past the end but with even-numbered repetitions 
reversed to produce a wave-like effect:
*    **    *
 *  *  *  *
  **    **

  
CAVEATS:

- Negative time values are not supported and will be clipped to zero.
  You are more than welcome to change that of course.
  
- You cannot change the Interpolator after it's been set. That is,
  you can't erase the contents or reset it to another preset.
  If that's what you want, just create another object from scratch.
  
*/





namespace LGC {



class Interpolator {
public:

	enum Repeat {
		NO_REPEAT, 
		SAWTOOTH,
		TRIANGLE
		};

	Interpolator();
	Interpolator( Repeat r );

	~Interpolator() {};
	
	float Value( float time );
	
	void AddPoint(float time, float value);
		
	void Clear();
	
	// EASY PRESETS: --------------------------------------------------------
	void Preset_Gradient	( float time, float start, float end );	
	void Preset_V			( float time, float end, float mid );	
	void Preset_Alternate	( float time, float on, float off );
	void Preset_BlinkFast	( float time, float on, float off );
	void Preset_BlinkSlow	( float time, float on, float off );
	
private:
	typedef std::map<float,float>::iterator point_iter;
	
	std::map<float,float> points;
	Repeat repeat;
	float maxtime;
	
};



	
} // end namespace LGC	




#endif


