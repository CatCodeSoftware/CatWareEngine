#include "Collision.h"

#include <cmath>

namespace CatWare
{
	namespace Physics
	{
		CollisionInfo TestCollision( Collider* a, Collider* b )
		{
			return CollisionInfo( );
		}

		CollisionInfo TestCollisionCircleCircle( CircleCollider* a, CircleCollider* b )
		{
			CollisionInfo ci;

			double distance = std::sqrt( ( b->position.x - a->position.x ) * ( b->position.x - a->position.x ) +
				( b->position.y - a->position.y ) * ( b->position.y - a->position.y ) );

			if ( distance <= a->radius + b->radius )
			{
				ci.hasCollision = true;
				ci.depth = distance - ( a->radius + b->radius );

				// Get point B
				double rotationB = b->position.GetRotationTo( a->position );
				Vector2D normalized = Vector2D::Normalize( rotationB, a->radius );
				ci.pointB = { a->position.x + normalized.x, a->position.y + normalized.y };

				// Same for point A
				double rotationA = a->position.GetRotationTo( b->position );
				Vector2D normalizedA = Vector2D::Normalize( rotationA, b->radius );
				ci.pointA = { b->position.x + normalizedA.x, b->position.y + normalizedA.y };

				ci.normalized = Vector2D::Normalize( ci.pointB.GetRotationTo( ci.pointB ), ci.pointB.GetDistanceTo( a->position ) );
			}
			else
			{
				ci.hasCollision = false;
			}

			return ci;
		}

		CollisionInfo TestCollisionRectRect( RectCollider* a, RectCollider* b )
		{
			CollisionInfo ci;

			if ( a->position.x + a->size.x >= b->position.x &&
				a->position.x <= b->position.x + b->size.x && 
				a->position.y + a->size.y >= b->position.y &&
				a->position.y <= b->position.y + b->size.y
				)
			{
				ci.hasCollision = true;

				Vector2D aCenenterPos = { a->position.x + a->size.x / 2, a->position.y + a->size.y / 2 };
				Vector2D bCenenterPos = { b->position.x + b->size.x / 2, b->position.y + b->size.y / 2 };

				ci.pointA = aCenenterPos;
				ci.pointB = bCenenterPos;
				ci.normalized = Vector2D::Normalize( bCenenterPos.GetRotationTo( aCenenterPos ), bCenenterPos.GetDistanceTo( aCenenterPos ) );
			}
			return ci;
		}

		CollisionInfo TestCollisionRectPoint( RectCollider* a, PointCollider* b )
		{
			return CollisionInfo( );
		}

		CollisionInfo TestCollisionCirclePoint( CircleCollider* a, PointCollider* b )
		{
			return CollisionInfo( );
		}
	}
}