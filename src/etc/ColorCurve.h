#ifndef COLOR_CURVE_H
#define COLOR_CURVE_H

#include "Interpolator.h"



namespace LGC {



class ColorCurve {

public:

	ColorCurve( Interpolator::Repeat repeat = Interpolator::TRIANGLE );

	~ColorCurve() {};
	
	void AddPoint(float time, float r=1.0, float g=1.0, float b=1.0, float a=1.0);
		
	inline float R( float time ) { return r.Value(time); }
	inline float G( float time ) { return g.Value(time); }
	inline float B( float time ) { return b.Value(time); }
	inline float A( float time ) { return a.Value(time); }
	
private:

	Interpolator r,g,b,a;
	
	};

	
} // end namespace LGC	



#endif


