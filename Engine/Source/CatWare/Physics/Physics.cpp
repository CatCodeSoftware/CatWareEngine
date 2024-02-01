#include "Physics.h"

#include <glm/glm.hpp>
#include <math.h>
#include <thread>


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
		body->collisionCallback = nullptr;
		bodiesToRemove.emplace( body );
	}

	void PhysicsWorld::RemoveItems( )
	{
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
			HandleCollisions( physicsBodies, deltaTime, substeps );
		}
	}

	void PhysicsWorld::HandleCollisions( std::vector<PhysicsBody*> physicsBodies, float deltaTime, int substeps )
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
							dynamicBody->position -= collisionInfo.normal / 2;
							dynamicBody2->position += collisionInfo.normal / 2;

							Vector2D relativeVeloctity = dynamicBody2->velocity - dynamicBody->velocity;

							float e = fmin( dynamicBody->restitution, dynamicBody2->restitution );
							float j = -( 1.0f + e ) * Vector2D::Dot( relativeVeloctity, collisionInfo.normal );
							j /= ( 1.0f / dynamicBody->mass ) + ( 1.0f / dynamicBody2->mass );

							dynamicBody->velocity -= collisionInfo.normal * ( j / dynamicBody->mass );
							dynamicBody2->velocity += collisionInfo.normal * ( j / dynamicBody2->mass );

							if ( physicsBody->collisionCallback != nullptr )
								physicsBody->collisionCallback( collisionInfo, physicsBody, physicsBody2 );
							if ( physicsBody2->collisionCallback != nullptr )
								physicsBody2->collisionCallback( collisionInfo, physicsBody2, physicsBody );
						}
					} else if ( physicsBody2->GetType( ) == BodyType::SURFACE )
					{
						SurfaceBody* surface = ( SurfaceBody* ) physicsBody2;

						// apply friction to the surface
						dynamicBody->force -=
							( dynamicBody->velocity * ( dynamicBody->mass * 10 ) * surface->frictionCoefficient );
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
				AABBCollider* collider = ( AABBCollider* ) ( ( DynamicBody* ) body )->GetCollider( );

				Renderer::DrawRect( collider->position, collider->size, Color( 255, 0, 0, 100 ), 0 );
			}
		}
	}

	PhysicsBody* PhysicsWorld::CastRay( Vector2D rayOrigin, Vector2D rayEnd, float pixelsPerStep, bool ( *ContinueCallback )( PhysicsBody* body ) )
	{
		float rayLenght = rayOrigin.GetDistanceTo( rayEnd );
		float rayAngle = rayOrigin.GetRotationTo( rayEnd );
		Vector2D currentPosition = rayOrigin;

		PointCollider* pointCollider = new PointCollider( currentPosition );

		while ( currentPosition.GetDistanceTo( rayOrigin ) < rayLenght )
		{
			pointCollider->position = currentPosition;

			for ( PhysicsBody* body : physicsBodies )
			{
				if ( body->GetType( ) == BodyType::DYNAMIC )
				{
					CollisionInfo result = TestCollision( static_cast<DynamicBody*>( body )->GetCollider( ), pointCollider );

					if ( result.hasCollision && !ContinueCallback( body ) )
						return body;

					currentPosition -= Vector2D::Normalize( rayAngle, pixelsPerStep );

					// Renderer::DrawRect( currentPosition, { 1, 1 }, { 255, 255, 255, 255 } );
				}
			}
		}

		return nullptr;
	}

	void PhysicsWorld::Clean( )
	{
		for ( PhysicsBody *body : physicsBodies )
			delete body;

		physicsBodies.clear( );
	}

}
