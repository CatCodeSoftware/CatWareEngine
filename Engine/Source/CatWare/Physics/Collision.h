/**
* @file Collision.h
*
* @brief Contains collider types and collision checking functions.
*
* @author PointThink
*/

#pragma once

#include "CatWare/Core.h"
#include "CatWare/Types/Vector.h"

namespace CatWare
{
	enum class ColliderType
	{
		CIRCLE, RECT, POINT, UNKNOWN
	};

	class CATWARE_API Collider
	{
	public:
		Vector2D position;

		//! position relative to physics body
		Vector2D positionRelative;

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

	class CATWARE_API AABBCollider : public Collider
	{
	public:
		AABBCollider( Vector2D position, Vector2D size )
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
		double penetration = 0; // lenght of b to a
		Vector2D normal = { 0, 0 }; // b to a normalized
	};

	//! Generic collision check function. Accepts any collider.
	CATWARE_API CollisionInfo TestCollision( Collider* a, Collider* b );

	CATWARE_API CollisionInfo TestCollisionCircleCircle( CircleCollider* a, CircleCollider* b );
	CATWARE_API CollisionInfo TestCollisionRectRect( AABBCollider* a, AABBCollider* b );
	CATWARE_API CollisionInfo TestCollisionRectPoint( AABBCollider* a, PointCollider* b );
	CATWARE_API CollisionInfo TestCollisionCirclePoint( CircleCollider* a, PointCollider* b );
	CATWARE_API CollisionInfo TestCollisionPointPoint( PointCollider* a, PointCollider* b );
}