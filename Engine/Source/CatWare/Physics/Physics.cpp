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

		void PhysicsWorld::Update( )
		{
			for ( PhysicsObject* object : physicsObjects )
			{
				object->force = object->force + Vector2D( gravity.x * object->mass * GlobalTime::GetDeltaTime( ), gravity.y * object->mass * GlobalTime::GetDeltaTime( ) );

				if ( object->frictionEnabled )
				{
					// calculate friction
					object->force.x -= ( object->frictionCoefficient * ( object->mass * 10 ) * object->velocity.x ) * GlobalTime::GetDeltaTime( );
					object->force.y -= ( object->frictionCoefficient * ( object->mass * 10 ) * object->velocity.y ) * GlobalTime::GetDeltaTime( );
				}

				object->velocity = object->velocity + Vector2D( ( object->force.x / object->mass ) * GlobalTime::GetDeltaTime( ), ( object->force.y / object->mass ) * GlobalTime::GetDeltaTime( ) );

				object->transform->position.x += object->velocity.x * GlobalTime::GetDeltaTime( );
				object->transform->position.y += object->velocity.y * GlobalTime::GetDeltaTime( );

				object->force = { 0, 0 };
			}
		}
	}
}