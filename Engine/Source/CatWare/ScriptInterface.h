#pragma once

#ifdef CW_PLATFORM_WIN64
#	define CW_SCRIPT_EXPORT extern "C" __declspec(dllexport)
#else
#	define CW_SCRIPT_EXPORT extern "C"
#endif

namespace CatWare
{
	class Script
	{
	public:
		virtual void PreInit( CatWare::InitConfig* initConfig ) {}
		virtual void Start( ) {}
		virtual void Activate( ) {}
		virtual void Exit( ) {}
	};
}

#define CW_REGISTER_SCRIPT( className ) CW_SCRIPT_EXPORT className* GetCWScript( ) { return new className; }