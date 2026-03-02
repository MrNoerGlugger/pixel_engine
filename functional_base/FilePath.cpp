#include "FilePath.hpp"

#ifdef _WIN32
#include <sstream>
#include <windows.h>
#include <KnownFolders.h>
#include <shlobj.h>
#endif

#ifdef linux
#include <unistd.h>
#include <pwd.h>
#endif

path FilePath::filePath;
string application_name = "Game";

void set_application_name(string name) {
	application_name = name;
}

void FilePath::createFilePath() {
	//initialize filePath
	filePath = "";

#ifdef _WIN32
	//get documents folder
	wchar_t* documentsPath = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &documentsPath);
	std::wstringstream pathStream;
	pathStream << documentsPath;
	CoTaskMemFree(documentsPath);
	filePath += pathStream.str();

	//check "My Games"
	filePath += "/My Games";
	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);

	//check "NoerGames"
	filePath += "/PixelEngine";
 	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);

	//check application_name
	filePath += "/" + application_name;
 	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);
#endif

#ifdef linux
	//get linux home path
	const char *homedir = getpwuid(getuid())->pw_dir;
	filePath += homedir;

	//check .noer-games folder
	filePath += "/.pixel-engine/";

 	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);

	//check application_name folder
	filePath += application_name + "/";

 	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);
#endif
}