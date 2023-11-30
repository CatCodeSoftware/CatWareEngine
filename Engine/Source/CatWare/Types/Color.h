#pragma once

#include "Types.h"

namespace CatWare
{
	class Color
	{
	public:
		Color( UInt8 r, UInt8 g, UInt8 b, UInt8 a )
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color( )
		{
			r = 255;
			g = 255;
			b = 255;
			a = 255;
		}
		
		UInt8 r, g, b, a;
	};

	typedef Color Colour;
}