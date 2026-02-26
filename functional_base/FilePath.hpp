#pragma once

#include <fstream>
#include <filesystem>

using std::filesystem::path;

class FilePath
{
public:
	static path filePath;
	static void createFilePath();
};

