#pragma once

#define SDL_MAIN_HANDLED

#if defined(CW_PLATFORM_WIN64)
#	if defined(CATWARE_BUILD)
#		define CATWARE_API __declspec(dllexport)
#	else
#		define CATWARE_API __declspec(dllimport)
#	endif
#endif

namespace CatWare
{
	void CATWARE_API Init( );
}