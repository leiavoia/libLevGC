/** \file LogManager.h
 * Declares the LogManager class.
 *
 * @author <A HREF="mailto:vovansim@hotmail.com">Vovansim (aka Scorpion)</A>
 * @date September 13, 2003


 CHANGELOG:
 --------------------------
 0.0.2	January 17, 2004
 	- class is now completely static. no need for GetInstance() or creation of any objects to use this class.
	- add switch to allow putting messages to screen as well as files.
 0.0.1	September 13, 2003
 	- first draft. works
 */



#ifndef LOG_MANAGER
#define LOG_MANAGER

#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdarg.h>



namespace LGC {



/** Enumerates the possible types of messages that can be logged by
 * the manager. Each different type will write the messages to its own
 * separate file. That is true for all but the FatalError-type
 * messages, which will be treated as urgent and redirected straight
 * to the stderr stream.
 */
typedef enum tagLogEntryType {
	LOGMSG_APP, /**< These would be the application messages. Not necessarily errors, but things pertinent to the application logic-related things. */
	LOGMSG_GRAPHIC,    /**< These are the messages related to graphics and GUI. Failed to load an image? Not enough video memory? Too many tiles? Clicked on a bad button? It belongs here. */
	LOGMSG_WARNING,     /**< These are miscellaneous warnings. Normally, everything should fit in the above categories, but what if I forgot something? Put it here. */
	LOGMSG_DEBUG,       /**< The good ol' debug messages... A pod cannot find his way around the map? Dump his info in here! */
	LOGMSG_FATAL,   /**< If something's gone terribly wrong, and you aren't even sure you can still write to disk, stick and error in here. It will be redirected straight to the standard error output. */
	LOGMSG_ERROR /**< Non-fatal errors.*/
} LogEntryType;

/**
 * The LogManager class allows for easy manipulation of all kinds of
 * application logging.  The most basic functionality of the class
 * resides in the overloaded Write method which allows the user to
 * write messages to the specified log.

 * @note The log manager uses a system of predefined strings that can
 * be used to write common log messages. Therefore, it requires a
 * messages.txt file to be located in the same folder as the
 * executable. If the text file is not present, pre-defined messages
 * are not available, but logging can still be done by passing a
 * custom value to the overloaded Write method.
 */
class LogMgr {
public:
	/** Writes the message passed in into the appropriate log
	 * file. Note that this method supports parameter replacement,
	 * and thus can be used just like, say, printf: one can pass
	 * it a format string and some replacement arguments, which
	 * would result in correctly formatted string being written to
	 * the log file. */
	static void Write(LogEntryType messageType, const char *msg, ...);

	/** Writes a pre-defined message into the appropriate log file. */
	static void Write(LogEntryType messageType, unsigned long msgID, ...);

	static void DisableDebugOutput( bool x ) { disable_debug_output = x; }

protected:
	LogMgr();
	~LogMgr();

	// Output streams for different message types
	static std::ofstream applicationStream;
	static std::ofstream graphicsStream;
	static std::ofstream warningStream;
	static std::ofstream debugStream;
	static std::ofstream errorStream;

	/** A vector of pre-defined message strings */
	static std::vector<std::string> messageStrings;

	/** Loads the pre-defined message string from a file on disk */
	static bool LoadStrings();

	static bool write_to_file;
	static bool write_to_stdout;

	static void WriteToScreen(bool x) { write_to_stdout = x; }
	static void WriteToFile(bool x) { write_to_file = x; }

	static bool disable_debug_output;
};




} // end namespace LGC



#endif


