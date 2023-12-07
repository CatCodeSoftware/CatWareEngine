#pragma once

#include <string>

#include "CatWare/Core.h"
#include "Console.h"

namespace CatWare
{
	class CATWARE_API DebugUI
	{
	public:
		inline static bool enabled = false;

		inline static bool consoleEnabled = false;
		inline static bool postProcessUIEnabled = false;

		static void Draw( );

		static Console* GetConsole( ) { return &console; }
	private:
		inline static Console console;
	};
}
