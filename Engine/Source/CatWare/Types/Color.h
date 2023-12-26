#pragma once

#include "Types.h"

namespace CatWare
{
	class Color
	{
	public:
		Color( float r, float g, float b, float a )
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
		
		float r, g, b, a;
	};

	typedef Color Colour;
}