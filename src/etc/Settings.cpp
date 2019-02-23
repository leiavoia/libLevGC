#include "Settings.h"
#include <iostream>
#include <fstream>


namespace LGC {


SettingsMgr* SettingsMgr::Instance() {
	static SettingsMgr x = SettingsMgr();
	return &x;
	}

void SettingsMgr::LoadSettings( const std::string& filename ) {

	std::ifstream file(filename.c_str());

	if (file.bad()) { LogMgr::Write(LOGMSG_ERROR, "Options file '%s' stream was corrupt or bad.", filename.c_str()); }
	if (!file.is_open()) { LogMgr::Write(LOGMSG_ERROR, "Options file '%s' could not be opened.", filename.c_str()); }

	std::string line;
	while ( file.good() && std::getline(file, line) ) {
		std::string first, second, temp;
		std::stringstream ss(line);
		ss >> first;
		while ( ss >> temp ) {
			if ( !second.empty() ) { second += " "; }
			second += temp;
			}
		//if (first[0] == '#') {continue;} // comments
		option_map.insert(  std::pair<std::string, std::string>(first,second)  );
		}
	}



void SettingsMgr::DumpSettings( const std::string& filename ) {

	std::ofstream file(filename.c_str());

	if (file.bad()) { LogMgr::Write(LOGMSG_ERROR, "Options file '%s' stream was corrupt or bad.", filename.c_str()); }
	if (!file.is_open()) { LogMgr::Write(LOGMSG_ERROR, "Options file '%s' could not be opened.", filename.c_str()); }

	for (std::map<std::string, std::string>::iterator i = option_map.begin(); i != option_map.end(); i++) {
		if (!file.good()) {
			LogMgr::Write(LOGMSG_ERROR, "Options file writing stream broken. Aborted writing.", filename.c_str());
			break;
			}
		if (i->first.empty() || i->second.empty()) { continue; }
		file << i->first << '\t' << i->second << std::endl;
		}
	}


// specialization for string types
template <>
std::string SettingsMgr::GetSetting<std::string>( const std::string& option ) {
	if ( option_map.find(option) == option_map.end() ) {
		LogMgr::Write(LOGMSG_ERROR, "No option '%s' found in options file. Please check spelling and case.", option.c_str());
		}
	return option_map[option];
	}

// specialization for string types
template <>
void SettingsMgr::SetSetting<std::string>( std::string key, std::string val ) {
	option_map[key] = val;
	}



} // end of namespace LGC






