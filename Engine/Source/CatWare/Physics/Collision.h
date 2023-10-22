#pragma once

#include "CatWare/Core.h"
#include "CatWare/Utils/Vector.h"

namespace CatWare
{
	namespace Physics
	{
		enum class ColliderType
		{
			CIRCLE, RECT, POINT, UNKNOWN
		};

		class CATWARE_API Collider
		{
		public:
			Vector2D position;
			ColliderType colliderType = ColliderType::UNKNOWN;
		};

		class CATWARE_API CircleCollider : public Collider
		{
		public:
			CircleCollider( Vector2D position, double radius )
			{ 
				this->position = position;
				this->radius = radius;
				colliderType = ColliderType::CIRCLE;
			}

			double radius;
		};

		class CATWARE_API RectCollider : public Collider
		{
		public:
			RectCollider( Vector2D position, Vector2D size )
			{
				this->position = position;
				this->size = size;
				colliderType = ColliderType::RECT;
			}

			Vector2D size;
		};

		class CATWARE_API PointCollider : public Collider
		{
		public:
			PointCollider( Vector2D position )
			{
				this->position = position;
				this->colliderType = ColliderType::POINT;
			}
		};

		class CollisionInfo
		{
		public:
			bool hasCollision = false; // self explanitory
			double depth = 0; // lenght of b to a

			Vector2D normalized = { 0, 0 }; // b to a normalized

			Vector2D pointA = { 0, 0 }; // Furthest point of A into B
			Vector2D pointB = { 0, 0 }; // Furthest point of B into A
		};

		CATWARE_API CollisionInfo TestCollision( Collider* a, Collider* b );

		CATWARE_API CollisionInfo TestCollisionCircleCircle( CircleCollider* a, CircleCollider* b );
		CATWARE_API CollisionInfo TestCollisionRectRect( RectCollider* a, RectCollider* b );
		CATWARE_API CollisionInfo TestCollisionRectPoint( RectCollider* a, PointCollider* b );
		CATWARE_API CollisionInfo TestCollisionCirclePoint( CircleCollider* a, PointCollider* b );
	}
}