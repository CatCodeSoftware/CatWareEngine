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

	void ( *fptrPreInit )( CatWare::InitConfig* initConfig ) = nullptr;
	void ( *fptrPostInit )( ) = nullptr;
	void ( *fptrActivate )( ) = nullptr;
	void ( *fptrDeInit )( ) = nullptr;

private:
#ifdef CW_PLATFORM_WIN64
	HMODULE dll;
#else
	void* dll;
#endif
};

std::vector<Script> LoadScripts( const std::string& scriptFolder );