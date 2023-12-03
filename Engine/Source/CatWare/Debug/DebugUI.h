#pragma once

#include "CatWare/Core.h"

namespace CatWare
{
	class CATWARE_API DebugUI
	{
	public:
		inline static bool enabled = true;

		inline static bool postProcessUIEnabled = false;

		static void Draw( );
	};
}
