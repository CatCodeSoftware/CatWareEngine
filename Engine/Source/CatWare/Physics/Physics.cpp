#include "Physics.h"

#include "..\Utils\Time.h"

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
			for ( std::vector<PhysicsObject*>::iterator it = physicsObjects.begin( ); it != physicsObjects.end( ); it++ )
			{
				if ( *it == physicsObject )
				{
					physicsObjects.erase( it );
				}
			}
		}

		void PhysicsWorld::Update( )
		{
			for ( PhysicsObject* object : physicsObjects )
			{
				object->force = object->force + Vector2D( gravity.x * object->mass, gravity.y * object->mass );
				object->velocity = object->velocity + Vector2D( ( object->force.x / object->mass ) * GlobalTime::GetDeltaTime( ), ( object->force.y / object->mass ) * GlobalTime::GetDeltaTime( ) );

				object->transform->position.x += object->velocity.x * GlobalTime::GetDeltaTime( );
				object->transform->position.y += object->velocity.y * GlobalTime::GetDeltaTime( );

				object->force = { 0, 0 };
			}
		}
	}
}