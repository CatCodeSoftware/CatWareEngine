#include "Error.h"

#include "CatWare/Debug/Debug.h"

#include <SDL.h>

#if defined(CW_PLATFORM_WIN64)
#	include <Windows.h>
#endif

namespace CatWare
{
	void FatalError( std::string file, int line, std::string message )
	{
		bool shouldExit = true;

		#if defined(CW_PLATFORM_WIN64)
		if ( MessageBoxA( NULL,
			( "Fatal error occured in file " + file + " in line " + std::to_string( line ) + "\n\n" + message + "\n\nContinue anyway?" ).c_str( ),
			"CatWare",
			MB_ICONSTOP | MB_YESNO ) == IDYES
			)
		{
			shouldExit = false;
		}
		#else
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR, "CatWare Error",
				( "Fatal error occured in file " + file + " in line " + std::to_string( line ) + "\n\n" + message ).c_str( ),
				NULL );
		#endif

		if( shouldExit )
			exit( 1 );
	}
}