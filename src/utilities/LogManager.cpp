/** \file LogMgr.cpp
 * Defines the LogMgr class.
 *
 * @author <A HREF="mailto:vovansim@hotmail.com">Vovansim (aka Scorpion)</A>
 * @version Version 0.0.2
 * @date January 17, 2004
 */

#include "LogManager.h"
//#include <stdlib.h>


namespace LGC {




std::ofstream LogMgr::applicationStream;
std::ofstream LogMgr::graphicsStream;
std::ofstream LogMgr::warningStream;
std::ofstream LogMgr::debugStream;
std::ofstream LogMgr::errorStream;
std::vector<std::string> LogMgr::messageStrings;
bool LogMgr::write_to_file = false;
bool LogMgr::write_to_stdout = true;
bool LogMgr::disable_debug_output = true;

//============================================= LogMgr::LogMgr ======//
LogMgr::LogMgr() {
	if(!LoadStrings()) {
		Write(LOGMSG_WARNING, "LogMgr: Failed to load pre-defined strings. Check existence of \"messages.txt\".");
		}

	Write(LOGMSG_APP, "Log Manager Initialized");
}

//============================================ LogMgr::~LogMgr ======//
LogMgr::~LogMgr() {
	Write(LOGMSG_APP, "Log Manager Destroyed");

	messageStrings.clear();

	if(applicationStream.is_open()) {
		applicationStream.close();
		}

	if(graphicsStream.is_open()) {
		graphicsStream.close();
		}

	if(warningStream.is_open()) {
		warningStream.close();
		}

	if(debugStream.is_open()) {
		debugStream.close();
		}
	if(errorStream.is_open()) {
		errorStream.close();
		}
	}

//======================================= bool LogMgr::LoadStrings ======//
bool LogMgr::LoadStrings() {
	std::ifstream in("messages.txt");

	if(!in.is_open()) {
		return false;
		}

	while(!in.eof()) {
		char szBuf[1024];
		in.getline(szBuf,1024);
		messageStrings.push_back(szBuf);
		}

	return true;
	}

//============================================= void LogMgr::Write ======//
void LogMgr::Write(LogEntryType messageType, const char *msg, ...) {
	va_list args;
	va_start(args,msg);

	char buffer[1024];
	vsprintf(buffer, msg, args);

	if(messageType == LOGMSG_FATAL) {
		fprintf(stderr, "*** FATAL ERROR ***\n");
		fprintf(stderr, buffer);
		fprintf(stderr, "\n");
		return;
		}

	switch(messageType) {
		case LOGMSG_DEBUG:
			if ( !disable_debug_output ) {
				if (write_to_file) {
					if(!debugStream.is_open()) {
						debugStream.open("debug.log");
						}
					debugStream << "DEBUG: " << buffer << std::endl << std::flush;
					}
				if (write_to_stdout) {
					std::cout << "DEBUG: " << buffer << std::endl << std::flush;
					}
				}
			break;

		case LOGMSG_GRAPHIC:
			if (write_to_file) {
				if(!graphicsStream.is_open()) {
					graphicsStream.open("graphics.log");
					}
				graphicsStream << "GRAPHICS: " << buffer << std::endl << std::flush;
				}
			if (write_to_stdout) {
				std::cout << "GRAPHICS: " << buffer << std::endl << std::flush;
				}
			break;

		case LOGMSG_WARNING:
			if (write_to_file) {
				if(!warningStream.is_open()) {
					warningStream.open("warnings.log");
					}
				warningStream << "WARNING: " << buffer << std::endl << std::flush;
				}
			if (write_to_stdout) {
				std::cout << "WARNING: " << buffer << std::endl << std::flush;
				}
			break;

		case LOGMSG_ERROR:
			if (write_to_file) {
				if(!errorStream.is_open()) {
					errorStream.open("errors.log");
					}
				errorStream << "ERROR: " << buffer << std::endl << std::flush;
				}
			if (write_to_stdout) {
				std::cout << "ERROR: " << buffer << std::endl << std::flush;
				}
			break;

		case LOGMSG_APP:
		default:
			if (write_to_file) {
				if(!applicationStream.is_open()) {
					applicationStream.open("application.log");
					}
				applicationStream << "PROGRAM: " << buffer << std::endl << std::flush;
				}
			if (write_to_stdout) {
				std::cout << "PROGRAM: " << buffer << std::endl << std::flush;
				}
			break;
		}

	va_end(args);
	}

//============================================= void LogMgr::Write ======//
void LogMgr::Write(LogEntryType messageType, unsigned long msgID, ...) {
	va_list args;
	va_start(args, msgID);

	char buffer[1024];
	vsprintf(buffer, messageStrings[msgID].c_str(), args);

	Write(messageType, buffer);

	va_end(args);
}

// ===========================================================================//





} // end namespace LGC


