#pragma once

#include <fstream>
#include <filesystem>
#include <string>

using std::filesystem::path;
using std::string;

class FilePath
{
	static string application_name;
public:
	static path filePath;
	static void set_application_name(string name);
	static void createFilePath();
};

