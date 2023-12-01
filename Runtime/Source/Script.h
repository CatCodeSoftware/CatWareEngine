#pragma once

#include <CatWare.h>

#include <string>

#include <Windows.h>

class Script
{
public:
	Script( std::string dllPath );

	void ( *fptrPreInit )( CatWare::InitConfig* initConfig ) = nullptr;
	void ( *fptrPostInit )( ) = nullptr;
	void ( *fptrActivate )( ) = nullptr;
	void ( *fptrDeInit )( ) = nullptr;

private:
	HMODULE dll;
};

std::vector<Script> LoadScripts( std::string scriptFolder );