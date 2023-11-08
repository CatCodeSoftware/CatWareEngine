/**
* @file Core.h
* 
* @brief Contains macro definitions. Not important to the end user
* 
* @author PointThink
*/

#pragma once

#define SDL_MAIN_HANDLED

#if defined(CW_PLATFORM_WIN64)
#	if defined(CATWARE_BUILD)
#		define CATWARE_API __declspec(dllexport)
#	else
#		define CATWARE_API __declspec(dllimport)
#	endif
#else
#	error Unsupported platform
#endif
