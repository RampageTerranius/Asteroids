#include "Misc Functions.h"

#include <Windows.h>
#include <PathCch.h>

// Gets the full path of the EXE file (minus the exe name).
std::string GetEXEPath()
{
	char exePath[MAX_PATH];

	GetModuleFileNameA(NULL, exePath, sizeof(exePath));

	std::string tempStr = exePath;

	size_t pos = tempStr.find_last_of("\\");

	if (pos != std::string::npos)
		return tempStr.substr(0, pos + 1);

	return "";
}