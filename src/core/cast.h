#ifndef _CAST_H
#define _CAST_H

#include <sstream>


namespace LGC {




namespace cast {
	// Just a little exception to throw in case something goes wrong.
	// Note here, that you may want to derive from std::bad_cast or
	// something, just to make sense out of this class.
	class BadCast {};

	//Local static stream to use when casting.
	static std::stringstream stream;

	// This is where the magic happens. You can cast anything into
	// anything else, elthough of course casting say float to double
	// would be silly using this.
	template< typename Target, typename Source >
	Target lexical_cast(Source arg) {
		//Create a placeholder for the result, of the type according to the template.
		Target result;

		//Clear stream, so if there's old junk in there, it goes away.
		stream.clear();

		//Stringinize the argument, and check that everything went along fine.
		stream << arg;
		if (stream.fail()) { throw BadCast(); }

		//Convert the string to whatever it is we want here:
		stream >> result;
		if (stream.fail()) { throw BadCast(); }

		//Return the result:
		return result;
	}


} // end namespace cast

} // end namespace LGC


#endif
