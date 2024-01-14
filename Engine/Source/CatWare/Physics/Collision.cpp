#include "Collision.h"

#include <cmath>

#include "CatWare/Debug/Debug.h"

namespace CatWare
{
	CollisionInfo TestCollision( Collider* a, Collider* b )
	{
		if ( a->colliderType == ColliderType::RECT && b->colliderType == ColliderType::RECT ) { return TestCollisionRectRect( (AABBCollider* ) a, ( AABBCollider* ) b ); }
		else if ( a->colliderType == ColliderType::CIRCLE && b->colliderType == ColliderType::CIRCLE ) { return TestCollisionCircleCircle( ( CircleCollider* ) a, ( CircleCollider* ) b ); }

		return { };
	}

	CollisionInfo TestCollisionCircleCircle( CircleCollider* a, CircleCollider* b )
	{
		CollisionInfo ci;

		double distance = std::sqrt( ( b->position.x - a->position.x ) * ( b->position.x - a->position.x ) +
			( b->position.y - a->position.y ) * ( b->position.y - a->position.y ) );

		if ( distance <= a->radius + b->radius )
		{
			ci.hasCollision = true;
			ci.penetration = distance - ( a->radius + b->radius );

			ci.normal = Vector2D::Normalize( a->position.GetRotationTo( b->position ), 1 );
		}
		else
		{
			ci.hasCollision = false;
		}

		return ci;
	}

	CollisionInfo TestCollisionRectRect( AABBCollider* a, AABBCollider* b )
	{
		CollisionInfo ci;

		// I have no idea what any of this does - PT
		Vector2D aCenter = a->position + ( a->size / Vector2D( 2, 2 ) );
		Vector2D bCenter = b->position + ( b->size / Vector2D( 2, 2 ) );

		Vector2D n = aCenter - bCenter;

		float aExtent = ( ( a->position.x + a->size.x ) - a->position.x ) / 2;
		float bExtent = ( ( b->position.x + b->size.x ) - b->position.x ) / 2;

		float xOverlap = aExtent + bExtent - abs( n.x );

		if ( xOverlap > 0 )
		{
			aExtent = ( ( a->position.y + a->size.y ) - a->position.y ) / 2;
			bExtent = ( ( b->position.y + b->size.y ) - b->position.y ) / 2;

			float yOverlap = aExtent + bExtent - abs( n.y );

			if ( yOverlap > 0 )
			{
				if ( xOverlap > yOverlap )
				{
					if ( n.y < 0 )
						ci.normal = { 0, 1 };
					else
						ci.normal = { 0, -1 };

					ci.penetration = yOverlap;
					ci.hasCollision = true;
				}
				else
				{
					if ( n.x < 0 )
						ci.normal = { 1, 0 };
					else
						ci.normal = { -1, 0 };

					ci.penetration = xOverlap;
					ci.hasCollision = true;
				}
			}

			ci.normal = ci.normal;
		}

		return ci;
	}

	CollisionInfo TestCollisionRectPoint( AABBCollider* a, PointCollider* b )
	{
		return CollisionInfo( );
	}

	CollisionInfo TestCollisionCirclePoint( CircleCollider* a, PointCollider* b )
	{
		return CollisionInfo( );
	}
}