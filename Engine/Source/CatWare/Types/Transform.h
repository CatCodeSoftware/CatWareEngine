#pragma once

#include "Vector.h"

namespace CatWare
{
	class Transform
	{
	public:
		Transform( Vector2D position = Vector2D( 0, 0 ), Vector2D size = Vector2D( 0, 0 ), float rotation = 0, int layer = 0 )
		{
			this->position = position;
			this->size = size;
			this->rotation = rotation;
			this->layer = layer;
		}

		Vector2D position = { 0, 0 };
		Vector2D size = { 0, 0 };

		Vector2D rotationPoint = { 0, 0 };
		float rotation = 0;

		int layer = 0;
	};
}