#include "Collision.h"

#include <cmath>

#include "CatWare/Debug/Debug.h"

namespace CatWare
{
	namespace Physics
	{
		CollisionInfo TestCollision( Collider* a, Collider* b )
		{
			if ( a->colliderType == ColliderType::RECT && b->colliderType == ColliderType::RECT ) { return TestCollisionRectRect( (RectCollider* ) a, ( RectCollider* ) b ); }
			else if ( a->colliderType == ColliderType::CIRCLE && b->colliderType == ColliderType::CIRCLE ) { return TestCollisionCircleCircle( ( CircleCollider* ) a, ( CircleCollider* ) b ); }

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

				ci.depth = ci.pointB.GetDistanceTo( a->position );
				ci.normalized = Vector2D::Normalize( ci.pointB.GetRotationTo( ci.pointA ), ci.depth );
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

				// i apologize for all of these ifs, i couldn't find a better way - PT
				if ( a->position.x <= b->position.x )
				{
					ci.pointA.x = b->position.x;
				}
				else
				{
					ci.pointA.x = b->position.x + b->size.x;
				}

				if ( a->position.y <= b->position.y )
				{
					ci.pointA.y = b->position.y;
				}
				else
				{
					ci.pointA.y = b->position.y + b->size.y;
				}

				if ( b->position.x <= a->position.x )
				{
					ci.pointB.x = a->position.x;
				}
				else
				{
					ci.pointB.x = a->position.x + a->size.x;
				}

				if ( b->position.y <= a->position.y )
				{
					ci.pointB.y = a->position.y;
				}
				else
				{
					ci.pointB.y = a->position.y + a->size.y;
				}

				ci.normalized = Vector2D::Normalize( ci.pointB.GetRotationTo( ci.pointA ), ci.pointB.GetDistanceTo( ci.pointA ) );
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