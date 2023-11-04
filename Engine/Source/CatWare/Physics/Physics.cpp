#include "Physics.h"

#include "..\Utils\Time.h"

#include "CatWare/Debug/Debug.h"

namespace CatWare
{
	namespace Physics
	{
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
				object->force = object->force + Vector2D( gravity.x * object->mass, gravity.y * object->mass );

				if ( object->frictionEnabled )
				{
					// calculate friction
					//object->force.x -= ( object->frictionCoefficient * ( object->mass * 10 ) * object->velocity.x );
					//object->force.y -= ( object->frictionCoefficient * ( object->mass * 10 ) * object->velocity.y );
				}
			}
		}

		void PhysicsWorld::Update( )
		{
			for ( PhysicsObject* object : physicsObjects )
			{
				object->velocity.x += object->force.x / object->mass;
				object->velocity.y += object->force.y / object->mass;

				object->transform->position.x += object->velocity.x * GlobalTime::GetDeltaTime( );
				object->transform->position.y += object->velocity.y * GlobalTime::GetDeltaTime( );

				object->force = { 0, 0 };
			}
		}
	}
}