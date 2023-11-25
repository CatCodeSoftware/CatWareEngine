#include "Script.h"

#include <CatWare/Error.h>

#include <Windows.h>
#include <filesystem>

Script::Script( std::string dllPath )
{
	dll = LoadLibraryA( dllPath.c_str( ) );

	if ( dll == nullptr )
	{
		CW_ABORT( "Couldn't load script " + dllPath );
	}

	fptrPreInit = ( void ( * )( CatWare::InitConfig* ) ) GetProcAddress( dll, "PreInit" );
	fptrPostInit = ( void ( * )( ) ) GetProcAddress( dll, "PostInit" );
	fptrActivate = ( void ( * )( ) ) GetProcAddress( dll, "Activate" );
	fptrDeInit = ( void ( * )( ) ) GetProcAddress( dll, "DeInit" );

	if ( fptrPreInit == nullptr )
	{
		CW_ENGINE_LOG->Error( "Couldn't load script " + dllPath + " because the function \"PreInit\" could not be loaded" );
	}

	if ( fptrPostInit == nullptr )
	{
		CW_ENGINE_LOG->Error( "Couldn't load script " + dllPath + " because the function \"PostInit\" could not be loaded" );
	}

	if ( fptrDeInit == nullptr )
	{
		CW_ENGINE_LOG->Error( "Couldn't load script " + dllPath + " because the function \"DeInit\" could not be loaded" );
	}
}

std::vector<Script> LoadScripts( std::string scriptFolder )
{
	std::vector<Script> scripts;

	for ( auto& entry : std::filesystem::recursive_directory_iterator( scriptFolder ) )
	{
		if ( entry.path( ).string( ).ends_with( ".dll" ) )
		{
			CW_ENGINE_LOG->Info( "Loading script %s", entry.path( ).string( ).c_str( ) );
			scripts.push_back( Script( entry.path( ).string( ) ) );
		}
	}

	return scripts;
}