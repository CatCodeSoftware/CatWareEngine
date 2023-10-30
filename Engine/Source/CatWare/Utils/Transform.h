#pragma once

#include "Vector.h"

namespace CatWare
{
	class Transform
	{
	public:
		Vector2D position = { 0, 0 };
		Vector2D size = { 0, 0 };

		Vector2D rotationPoint = { 0, 0 };
		float rotation = 0;
	};
}