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

	//check "Game"
	filePath += "/Game";
 	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);
#endif

#ifdef linux
	//get linux home path
	const char *homedir = getpwuid(getuid())->pw_dir;
	filePath += homedir;

	//check .game folder
	filePath += "/.game/";

 	if (!std::filesystem::exists(filePath))
		std::filesystem::create_directory(filePath);
#endif
}