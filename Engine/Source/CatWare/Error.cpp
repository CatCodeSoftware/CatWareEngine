#include "Error.h"

#include "CatWare/Debug/Debug.h"

#include <SDL.h>

#include "Log.h"
#include "Types/Types.h"

#if defined( CW_PLATFORM_WIN64 )
#include <Windows.h>
#else
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#endif

namespace CatWare
{
	void CrashHandler( int sig )
	{
		std::string error;

		switch ( sig )
		{
		case 11:
			error = "SIGSEGV - Segmentation fault ) | Memory Error";
			break;
		case 8:
			error = "SIGFPE - Arithmetic exception | Division by 0 maybe?";
			break;
		}

		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "CatWare Crash", ( "The CatWare Engine crashed\n\n" + error ).c_str( ), nullptr );

		CW_ENGINE_LOG->Error( "Program crashed!" );
	}

	void FatalError( std::string file, int line, std::string message )
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR, "CatWare Error",
			( "Fatal error occured in file " + file + " in line " + std::to_string( line ) + "\n\n" + message )
				.c_str( ),
			nullptr );

#if defined( CW_DEBUG )
		// *( ( UInt8* ) NULL ) = NULL; // trigger a segfault to set off IDE debuggers
#endif

		CW_ENGINE_LOG->Error(
			( "Fatal error occured in file " + file + " in line " + std::to_string( line ) + "\n\n" + message )
				.c_str( ) );

		exit( 1 );
	}
} // namespace CatWare
