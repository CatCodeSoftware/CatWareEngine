#pragma once

#ifdef CW_PLATFORM_WIN64
#	define CW_SCRIPT_EXPORT extern "C" __declspec(dllexport)
#else
#	define CW_SCRIPT_EXPORT extern "C"
#endif
