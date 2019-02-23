#ifndef SETTINGSMGR_H
#define SETTINGSMGR_H

#include <string>
#include <sstream>
#include <map>
#include "../utilities/LogManager.h"


namespace LGC {


// handy shortcut
#define Settings SettingsMgr::Instance()


class SettingsMgr {
public:
	static SettingsMgr* Instance();

	void LoadSettings( const std::string& filename );

	void DumpSettings( const std::string& filename );

	template <class TYPE>
	TYPE GetSetting( const std::string& option ) {
		TYPE result;  // what we will be returning
		std::stringstream stream; //static stream to use when casting.
		stream.clear(); // clear any residual junk
		if ( option_map.find(option) == option_map.end() ) {
			LogMgr::Write(LOGMSG_ERROR, "No option '%s' found in options file. Please check spelling and case.", option.c_str());
			}
		stream << std::boolalpha << option_map[option]; // convert the option value into a stringstream
		stream >> result; // then shove it into the result. Presto!
		return result;
		}

	template <class TYPE>
	void SetSetting( std::string key, TYPE val ) {
		std::stringstream stream;
		stream.clear();
		stream << std::boolalpha << val;
		option_map[key] = stream.str();
		}

private:
	SettingsMgr(){}
	~SettingsMgr(){}
	std::map< std::string, std::string > option_map;
	};


// specialization for string types
template <> std::string SettingsMgr::GetSetting<std::string>( const std::string& option );
template <> void SettingsMgr::SetSetting<std::string>( std::string key, std::string val );


} // end namespace LGC


#endif







