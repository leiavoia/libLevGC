#include "ColorCurve.h"



namespace LGC {



ColorCurve::ColorCurve( Interpolator::Repeat repeat ) {
	r = Interpolator( repeat );
	g = Interpolator( repeat );
	b = Interpolator( repeat );
	a = Interpolator( repeat );
	}
	
	
void ColorCurve::AddPoint(float time, float r, float g, float b, float a) {
	this->r.AddPoint(time,r);	
	this->g.AddPoint(time,g);	
	this->b.AddPoint(time,b);	
	this->a.AddPoint(time,a);		
	}
	

} // end namespace LGC	




