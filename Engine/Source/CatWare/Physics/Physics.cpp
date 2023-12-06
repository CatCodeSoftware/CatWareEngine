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

	SurfaceBody::SurfaceBody( float frictionCoefficient, Collider* collider ) : PhysicsBody( BodyType::SURFACE )
	{
		this->frictionCoefficient = frictionCoefficient;
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
				dynamicBody->force.y += dynamicBody->mass * gravity;

				dynamicBody->velocity += dynamicBody->force / dynamicBody->mass * deltaTime;
				dynamicBody->position += dynamicBody->velocity * deltaTime;

				dynamicBody->force = { 0, 0 };

				// Collision checking, only brute force now
				for ( PhysicsBody* physicsBody2 : physicsBodies )
				{
					if ( physicsBody == physicsBody2 ) break;

					if ( physicsBody2->GetType( ) == BodyType::DYNAMIC )
					{
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
					else if ( physicsBody2->GetType( ) == BodyType::SURFACE )
					{
						SurfaceBody* surface = ( SurfaceBody* ) physicsBody2;

						// apply friction to the surface
						dynamicBody->force -= dynamicBody->velocity * ( dynamicBody->mass * 10 ) * surface->frictionCoefficient;
					}
				}
			}
		}
	}
}