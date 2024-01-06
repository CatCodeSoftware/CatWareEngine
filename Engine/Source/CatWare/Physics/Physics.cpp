#include "Physics.h"

#include <glm/glm.hpp>
#include <math.h>


#include "CatWare/Graphics/Renderer/Renderer.h"

namespace CatWare
{
	PhysicsBody::PhysicsBody( BodyType bodyType )
	{
		this->type = bodyType;
	}

	DynamicBody::DynamicBody( float mass, float restitution, Collider *collider ) :
		PhysicsBody( BodyType::DYNAMIC )
	{
		this->mass = mass;
		this->restitution = restitution;
		this->collider = collider;
	}

	SurfaceBody::SurfaceBody( float frictionCoefficient, Collider *collider ) :
		PhysicsBody( BodyType::SURFACE )
	{
		this->frictionCoefficient = frictionCoefficient;
		this->collider = collider;
	}

	void PhysicsWorld::AddBody( PhysicsBody *body )
	{
		physicsBodies.push_back( body );
	}

	void PhysicsWorld::RemoveBody( PhysicsBody *body )
	{
		bodiesToRemove.emplace( body );
	}

	void PhysicsWorld::RemoveItems( )
	{
		// Every signle fucking problem i've had in the past week was fucking vectors
		// I might be stupid. But removing elements from a vector that is being looped
		// through every frame is the most error prone operation you could do in c++.
		// If you do something wrong your debugger will tell you that it couldn't access
		// some random memory address in a random function because stack corruption or something
		// I hate vectors and i hate myself
		// For now i switched to recreating the vector without the removed elements
		std::vector<PhysicsBody*> tempVec = physicsBodies;
		physicsBodies.clear( );

		for ( PhysicsBody* physicsBody : tempVec )
		{
			bool add = true;
			for ( PhysicsBody* physicsBody2 : bodiesToRemove )
			{
				if ( physicsBody == physicsBody2 )
				{
					add = false;
					break;
				}
			}

			if ( add )
				physicsBodies.push_back( physicsBody );
		}

		bodiesToRemove.clear( );
	}

	void PhysicsWorld::Step( float deltaTime, unsigned substeps )
	{
		deltaTime /= substeps;

		// This code __WILL__ anger never-nesters
		for ( unsigned int i = 0; i < substeps; i++ )
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
					dynamicBody->GetCollider( )->position = dynamicBody->position;

					dynamicBody->force = { 0, 0 };

					// Collision checking, only brute force now
					for ( PhysicsBody* physicsBody2 : physicsBodies )
					{
						if ( physicsBody == physicsBody2 )
							continue;


						if ( physicsBody2->GetType( ) == BodyType::DYNAMIC )
						{
							DynamicBody* dynamicBody2 = ( DynamicBody* ) physicsBody2;

							// Update positions of colliders
							dynamicBody->GetCollider( )->position = dynamicBody->position;
							dynamicBody2->GetCollider( )->position = dynamicBody2->position;

							CollisionInfo collisionInfo =
								TestCollision( dynamicBody->GetCollider( ), dynamicBody2->GetCollider( ) );

							if ( collisionInfo.hasCollision )
							{
								if ( i == 0 )
								{
									if ( physicsBody->collisionCallback != nullptr )
										physicsBody->collisionCallback( collisionInfo, physicsBody, physicsBody2 );
									if ( physicsBody2->collisionCallback != nullptr )
										physicsBody2->collisionCallback( collisionInfo, physicsBody2, physicsBody );
								}

								dynamicBody->position -= collisionInfo.normal / 2;
								dynamicBody2->position += collisionInfo.normal / 2;

								Vector2D relativeVeloctity = dynamicBody2->velocity - dynamicBody->velocity;

								float e = fmin( dynamicBody->restitution, dynamicBody2->restitution );
								float j = -( 1.0f + e ) * Vector2D::Dot( relativeVeloctity, collisionInfo.normal );
								j /= ( 1.0f / dynamicBody->mass ) + ( 1.0f / dynamicBody2->mass );

								dynamicBody->velocity -= collisionInfo.normal * ( j / dynamicBody->mass );
								dynamicBody2->velocity += collisionInfo.normal * ( j / dynamicBody2->mass );
							}
						} else if ( physicsBody2->GetType( ) == BodyType::SURFACE )
						{
							SurfaceBody* surface = ( SurfaceBody* ) physicsBody2;

							// apply friction to the surface
							dynamicBody->force -=
								dynamicBody->velocity * ( dynamicBody->mass * 10 ) * surface->frictionCoefficient;
						}
					}
				}
			}
		}
	}

	void PhysicsWorld::DrawColliders( )
	{
		for ( PhysicsBody* body : physicsBodies )
		{
			if ( body->GetType( ) == BodyType::DYNAMIC )
			{
				RectCollider* collider = ( RectCollider* ) ( ( DynamicBody* ) body )->GetCollider( );

				Renderer::DrawRect( collider->position, collider->size, Color( 255, 0, 0, 100 ), 0 );
			}
		}
	}

	void PhysicsWorld::Clean( )
	{
		for ( PhysicsBody *body : physicsBodies )
			delete body;

		physicsBodies.clear( );
	}

}
