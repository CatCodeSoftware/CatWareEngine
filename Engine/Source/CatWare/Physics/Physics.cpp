#include "Physics.h"

#include "..\Utils\Time.h"

#include "CatWare/Debug/Debug.h"

namespace CatWare
{
	namespace Physics
	{
		void PhysicsObject::AttachCollider( Collider* collider, Vector2D relativePos )
		{
			collider->positionRelative = relativePos;
			colliders.push_back( collider );
		}

		void PhysicsObject::DetachCollider( Collider* collider )
		{
			for ( std::vector<Collider*>::iterator it = colliders.begin( ); it != colliders.end( ); )
			{
				if ( *it == collider )
				{
					colliders.erase( it );
					break;
				}
				else
				{
					it++;
				}
			}
		}

		void PhysicsObject::SetResponse( CollisionResponse* response )
		{
			this->response = response;
		}

		// PhysicsWorld

		void PhysicsWorld::AddObject( PhysicsObject* physicsObject )
		{
			physicsObjects.push_back( physicsObject );
		}

		void PhysicsWorld::RemoveObject( PhysicsObject* physicsObject )
		{
			for ( std::vector<PhysicsObject*>::iterator it = physicsObjects.begin( ); it != physicsObjects.end( ); )
			{
				if ( *it == physicsObject )
				{
					physicsObjects.erase( it );
					break;
				}
				else
				{
					it++;
				}
			}
		}

		void PhysicsWorld::Tick( )
		{
			for ( PhysicsObject* object : physicsObjects )
			{
				// update collider positions
				for ( Collider* collider : object->GetColliders( ) )
				{
					collider->position = object->transform->position + collider->positionRelative;
				}

				object->force = object->force + Vector2D( gravity.x * object->mass, gravity.y * object->mass );

				if ( object->frictionEnabled )
				{
					// calculate friction
					object->force.x -= ( object->frictionCoefficient * ( object->mass * 10 ) * object->velocity.x );
					object->force.y -= ( object->frictionCoefficient * ( object->mass * 10 ) * object->velocity.y );
				}

				object->velocity.x += ( object->force.x / object->mass ) / GlobalTime::ticksPerSecond * GlobalTime::modifier;
				object->velocity.y += object->force.y / object->mass / GlobalTime::ticksPerSecond * GlobalTime::modifier;

				// check collisions between objects
				for ( PhysicsObject* physObj1 : physicsObjects )
				{
					for ( PhysicsObject* physObj2 : physicsObjects )
					{
						if ( physObj1 == physObj2 || ( !physObj2->collidable || !physObj1->collidable ) ) break;

						ResolveCollisions( physObj1, physObj2 );
					}
				}
			}
		}

		void PhysicsWorld::Update( )
		{
			for ( PhysicsObject* object : physicsObjects )
			{
				object->transform->position.x += object->velocity.x * GlobalTime::GetDeltaTime( );
				object->transform->position.y += object->velocity.y * GlobalTime::GetDeltaTime( );

				object->force = { 0, 0 };

			}
		}

		void PhysicsWorld::ResolveCollisions( PhysicsObject* physObj1, PhysicsObject* physObj2 )
		{
			for ( Collider* c1 : physObj1->GetColliders( ) )
			{
				for ( Collider* c2 : physObj2->GetColliders( ) )
				{
					CollisionInfo ci = TestCollision( c1, c2 );

					if ( ci.hasCollision )
					{
						/*
						if ( physObj1->response != nullptr )
							physObj1->response->OnCollide( physObj1, physObj2, ci );

						if ( physObj2->response != nullptr )
							physObj1->response->OnCollide( physObj2, physObj1, ci );
						*/

						// Correct position

						if ( physObj1->movable )
							physObj1->transform->position = physObj1->transform->position - ci.normal;

						if ( physObj2->movable )
							physObj2->transform->position = physObj2->transform->position + ci.normal;

						// stop checking collision for these objects
						return;
					}
				}
			}
		}
	}
}