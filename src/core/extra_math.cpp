#include "extra_math.h"

namespace LGC {

int NextPowerOfTwo(int x) {
	int p = 2;
	while (p < x) {
	  	p *= 2;
		}
	return p;
	}

} // end namespace LGC

