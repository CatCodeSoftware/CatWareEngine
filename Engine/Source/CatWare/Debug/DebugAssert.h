#pragma once

#include <string>

// this does not quit the application and only shows up in debug mode
// if you want to trigger a fatal error use CW_ABORT from Error.h
#ifdef CW_DEBUG
#	define CW_ASSERT( condition, text) if ( condition ) { CatWare::Assert( __FILE__, __LINE__, text ); }
#else
#	define CW_ASSERT( condition, text)
#endif

namespace CatWare
{
	void Assert( std::string file, int line, std::string message );
}
