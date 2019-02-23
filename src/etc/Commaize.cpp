#include "Commaize.h"
#include <cctype>
#include <sstream>
#include <string>
#include <iomanip>

namespace LGC {


std::string commaize ( unsigned long val ) {
	std::string buff;
	int i = 0;

	// The workhorse algorithm. Use of std::string::insert
	// is okay since the string will not be very long even
	// for large unsigned long values. If you end up using
	// an arbitrary length number, changing this to use a
	// more suitable buffer container would be advisable
	do {
		if ( i != 0 && i % 3 == 0 ) { buff.insert ( buff.begin(), ',' ); }
		buff.insert ( buff.begin(), '0' + val % 10 );
		i++;
		}
	while ( ( val /= 10 ) != 0 );
	return buff;
	}

} // END NAMESPACE LGC


