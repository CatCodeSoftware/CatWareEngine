#include "Error.h"

#include "CatWare/Debug/Debug.h"

#include <SDL.h>

#include "Types/Types.h"

#if defined(CW_PLATFORM_WIN64)
#	include <Windows.h>
#endif

namespace CatWare
{
	void FatalError( std::string file, int line, std::string message )
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR, "CatWare Error",
			( "Fatal error occured in file " + file + " in line " + std::to_string( line ) + "\n\n" + message ).
			c_str( ),
			nullptr
		);

		#if defined(CW_DEBUG)
				// *( ( UInt8* ) NULL ) = NULL; // trigger a segfault to set off IDE debuggers
		#endif

		exit( 1 );
	}
}
