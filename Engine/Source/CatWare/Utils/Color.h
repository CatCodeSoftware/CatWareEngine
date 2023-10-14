#pragma once

#include "Types.h"

#define Colour Color // for the brits - PT

namespace CatWare
{
	class Color
	{
	public:
		Color( UInt8 r, UInt8 g, UInt8 b, UInt8 a );
		
		UInt8 r, g, b, a;
	};
}