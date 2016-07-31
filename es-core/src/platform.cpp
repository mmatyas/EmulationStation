#include "platform.h"
#include "Settings.h"
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <iostream>

#ifdef WIN32
#include <codecvt>
#include <shlobj.h>
#include <windows.h>
#endif

std::string getHomePath()
{
	std::string homePath;

	// this should give you something like "/home/YOUR_USERNAME" on Linux and "C:\Users\YOUR_USERNAME\" on Windows
	const char * envHome = getenv("HOME");
	if(envHome != nullptr)
	{
		homePath = envHome;
	}

#ifdef WIN32
	// but does not seem to work for Windows XP or Vista, so try something else
	if (homePath.empty()) {
		const char * envDir = getenv("HOMEDRIVE");
		const char * envPath = getenv("HOMEPATH");
		if (envDir != nullptr && envPath != nullptr) {
			homePath = envDir;
			homePath += envPath;

			for(unsigned int i = 0; i < homePath.length(); i++)
				if(homePath[i] == '\\')
					homePath[i] = '/';
		}
	}
#endif

	// convert path to generic directory seperators
	boost::filesystem::path genericPath(homePath);
	return genericPath.generic_string();
}

std::string getConfigDirectory()
{
	if (!Settings::getInstance()->getString("ConfigDirectory").empty())
		return Settings::getInstance()->getString("ConfigDirectory");

	boost::filesystem::path path;
#ifdef _WIN32
	CHAR my_documents[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	path = boost::filesystem::path(my_documents) / "EmulationStation";
#elif __APPLE__ && !defined(USE_XDG_OSX)
	const char* homePath = getenv("HOME");
	path = boost::filesystem::path(homePath);
	path /= "Library" / "Application Support" / "org.emulationstation.EmulationStation";
#else
	const char* envXdgConfig = getenv("XDG_CONFIG_HOME");
	if(envXdgConfig) {
		path = boost::filesystem::path(envXdgConfig);
	} else {
		const char* homePath = getenv("HOME");
		path = boost::filesystem::path(homePath);
		path /= boost::filesystem::path(".config");
	}
	path /= boost::filesystem::path("emulationstation");
#endif
	auto final_path = path.generic_string();
	Settings::getInstance()->setString("ConfigDirectory", final_path);
	return final_path;
}

int runShutdownCommand()
{
#ifdef WIN32 // windows
	return system("shutdown -s -t 0");
#else // osx / linux
	return system("sudo shutdown -h now");
#endif
}

int runRestartCommand()
{
#ifdef WIN32 // windows
	return system("shutdown -r -t 0");
#else // osx / linux
	return system("sudo shutdown -r now");
#endif
}

int runSystemCommand(const std::string& cmd_utf8)
{
#ifdef WIN32
	// on Windows we use _wsystem to support non-ASCII paths
	// which requires converting from utf8 to a wstring
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::wstring wchar_str = converter.from_bytes(cmd_utf8);
	return _wsystem(wchar_str.c_str());
#else
	return system(cmd_utf8.c_str());
#endif
}
