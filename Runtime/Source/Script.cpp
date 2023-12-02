#include "Script.h"

#include <string>
#include <filesystem>

#include <CatWare/Error.h>

typedef void ( *VoidFunc )( );

#ifdef CW_PLATFORM_WIN64

HMODULE LoadLib( std::string path )
{
	return LoadLibraryA( path.c_str( ) );
}

VoidFunc LoadFunc( HMODULE dllHandle, std::string funcName )
{
	return ( VoidFunc ) GetProcAddress( dllHandle, funcName.c_str( ) );
}

#else

#include <dlfcn.h>

VoidFunc LoadFunc( void* dllHandle, const std::string& funcName )
{
	return ( VoidFunc ) dlsym( dllHandle, funcName.c_str( ) );
}

void* LoadLib( const std::string& path )
{
	return dlopen( path.c_str( ), RTLD_NOW );
}

#endif

Script::Script( const std::string& dllPath )
{
	dll = LoadLib( dllPath );

	if ( dll == nullptr )
	{
		CW_ABORT( "Couldn't load script " + dllPath );
	}

	fptrPreInit = ( void ( * )( CatWare::InitConfig* ) ) LoadFunc( dll, "PreInit" );
	fptrPostInit = ( void ( * )( ) ) LoadFunc( dll, "PostInit" );
	fptrActivate = ( void ( * )( ) ) LoadFunc( dll, "Activate" );
	fptrDeInit = ( void ( * )( ) ) LoadFunc( dll, "DeInit" );

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

std::vector<Script> LoadScripts( const std::string& scriptFolder )
{
	std::vector<Script> scripts;

	for ( auto& entry : std::filesystem::recursive_directory_iterator( scriptFolder ) )
	{
		if ( entry.path( ).string( ).ends_with( ".dll" ) || entry.path( ).string( ).ends_with( ".so" ) )
		{
			CW_ENGINE_LOG->Info( "Loading script %s", entry.path( ).string( ).c_str( ) );
			scripts.emplace_back( entry.path( ).string( ) );
		}
	}

	return scripts;
}