#include "DebugAssert.h"

#include <SDL.h>

namespace CatWare
{
	void Assert( std::string file, int line, std::string message )
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_WARNING, "CatWare Assert",
			( "Assert trigerred in file " + file + " on line " + std::to_string( line ) + "\n\n" + message ).c_str( ),
			NULL
		);
	}
}
