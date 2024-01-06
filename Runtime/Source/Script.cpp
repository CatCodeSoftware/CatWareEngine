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

inline bool StringEndsWith( std::string string, std::string end )
{
	// Get the last characters of the string
	std::string stringEnd;

	for ( unsigned int i = 0; i < end.length( ); i++ )
		stringEnd += string[string.length( ) - end.length( ) + i];

	return stringEnd == end;
}

Script::Script( const std::string& dllPath )
{
	dll = LoadLib( dllPath );

	if ( dll == nullptr )
	{
		CW_ABORT( "Couldn't load script " + dllPath );
	}

	CatWare::Script* ( *scriptGetFunc )( ) = ( CatWare::Script* ( * )( ) ) LoadFunc( dll, "GetCWScript" );
	script = scriptGetFunc( );
}

std::vector<Script> LoadScripts( const std::string& scriptFolder )
{
	std::vector<Script> scripts;

	for ( auto& entry : std::filesystem::recursive_directory_iterator( scriptFolder ) )
	{
		if ( StringEndsWith( entry.path( ).string( ), ".dll") || StringEndsWith( entry.path( ).string( ), ".so") )
		{
			CW_ENGINE_LOG->Info( "Loading script %s", entry.path( ).string( ).c_str( ) );
			scripts.emplace_back( entry.path( ).string( ) );
		}
	}

	return scripts;
}