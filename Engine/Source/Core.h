#pragma once

#if defined(CATWARE_BUILD)
#	define CATWARE_API __declspec(dllexport)
#else
#	define CATWARE_API __declspec(dllimport)
#endif