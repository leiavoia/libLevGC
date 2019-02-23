

#ifndef EXTRAMATH_H
#define EXTRAMATH_H

#include <stdlib.h>


namespace LGC {




const float PI = 3.1415926f;

inline int RandomNum(int max /* up to but not incuding */ ) {
	return int(   max * (double)rand() / (double)RAND_MAX   );
	}

inline float DegToRad(float angle) {
	return angle * (PI / 180.0f);
	}

inline float RadToDeg(float angle) {
	return angle / (PI / 180.0f);
	}

// returns a power of two one-stop greater than the value supplied
int NextPowerOfTwo(int x);

template <class TYPE> TYPE Clamp( TYPE value, TYPE min, TYPE max) {
    	TYPE x = value;
    	if ( x > max ) { x = max; }
	if ( x < min ) { x = min; }
	return x;
	}

} // end namespace LGC



#endif



