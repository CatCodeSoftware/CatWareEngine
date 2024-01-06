#pragma once

#include <CatWare.h>

#include <string>

#ifdef CW_PLATFORM_WIN64
#	include <Windows.h>
#else
#endif

class Script
{
public:
	Script( const std::string& dllPath );

	CatWare::Script* script;

private:
#ifdef CW_PLATFORM_WIN64
	HMODULE dll;
#else
	void* dll;
#endif
};

std::vector<Script> LoadScripts( const std::string& scriptFolder );