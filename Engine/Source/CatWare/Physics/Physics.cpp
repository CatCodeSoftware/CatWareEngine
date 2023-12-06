#include "Physics.h"

#include <glm/glm.hpp>

namespace CatWare
{
	PhysicsBody::PhysicsBody( BodyType bodyType )
	{
		this->type = bodyType;
	}

	DynamicBody::DynamicBody( float mass, Collider* collider ) : PhysicsBody( BodyType::DYNAMIC )
	{
		this->mass = mass;
		this->collider = collider;
	}

	void PhysicsWorld::AddBody( PhysicsBody* body )
	{
		physicsBodies.push_back( body );
	}

	void PhysicsWorld::RemoveBody( PhysicsBody* body )
	{
		// loop through the vector until you find the body
		unsigned int index = 0;
		for ( PhysicsBody* body2 : physicsBodies )
		{
			if ( body == body2 )
			{
				// since we don't care about order we can use the switch and pop method
				unsigned int lastIndex = physicsBodies.size( ) - 1;

				PhysicsBody* tempBody = physicsBodies[lastIndex];

				physicsBodies[lastIndex] = physicsBodies[index];
				physicsBodies[index] = tempBody;

				delete body;
				physicsBodies.pop_back( );

				break;
			}

			index++;
		}
	}

	void PhysicsWorld::Step( float deltaTime, unsigned substeps )
	{
		for ( PhysicsBody* physicsBody : physicsBodies )
		{
			if ( physicsBody->GetType( ) == BodyType::DYNAMIC )
			{
				DynamicBody* dynamicBody = ( DynamicBody* ) physicsBody;

				// temporary gravity thingy
				dynamicBody->force.y += dynamicBody->mass * 600;

				dynamicBody->velocity += dynamicBody->force / dynamicBody->mass * deltaTime;
				dynamicBody->position += dynamicBody->velocity * deltaTime;

				// Collision checking, only brute force now
				for ( PhysicsBody* physicsBody2 : physicsBodies )
				{
					if ( physicsBody2->GetType( ) == BodyType::DYNAMIC )
					{
						if ( physicsBody == physicsBody2 ) break;

						DynamicBody* dynamicBody2 = ( DynamicBody* ) physicsBody2;

						// Update positions of colliders
						dynamicBody->GetCollider( )->position = dynamicBody->position;
						dynamicBody2->GetCollider( )->position = dynamicBody2->position;

						CollisionInfo collisionInfo = TestCollision( dynamicBody->GetCollider( ), dynamicBody2->GetCollider( ) );

						if ( collisionInfo.hasCollision )
						{
							dynamicBody->position -= collisionInfo.normal * collisionInfo.penetration;
							dynamicBody2->position += collisionInfo.normal * collisionInfo.penetration;

							break;
						}
					}
				}

				dynamicBody->force = { 0, 0 };
			}
		}
	}
}