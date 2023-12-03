#pragma once

#include <vector>
#include <string>

#include "CatWare/Core.h"

namespace CatWare
{
	class CATWARE_API DebugUI
	{
	public:
		inline static bool enabled = true;

		inline static bool consoleEnabled = false;
		inline static bool postProcessUIEnabled = false;

		static void Draw( );

		static void PrintToConsole( std::string text );

	private:
		inline static char commandBuffer[256];
		inline static std::vector<std::string> consoleContents;
	};
}
