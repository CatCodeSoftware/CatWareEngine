/**
* @file Core.h
* 
* @brief Contains macro definitions. Not important to the end user
* 
* @author PointThink
*/

#pragma once

#if !defined(SDL_MAIN_HANDLED)
#   define SDL_MAIN_HANDLED
#endif

#if defined(CW_PLATFORM_WIN64)
#	if defined(CATWARE_BUILD)
#		define CATWARE_API __declspec(dllexport)
#		define IMGUI_API __declspec(dllexport)
#	else
#		define CATWARE_API __declspec(dllimport)
#		define IMGUI_API __declspec(dllimport)
#	endif
#elif defined(CW_PLATFORM_UNIX64)
#   define CATWARE_API
#else
#	error Unsupported platform
#endif
