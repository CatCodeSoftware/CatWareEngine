#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "Collision.h"
#include "CatWare/Types/Vector.h"
#include "CatWare/Core.h"

namespace CatWare
{
	enum class BodyType
	{
		UNASSIGNED, // Invalid body
		DYNAMIC, // A standard rigidbody
		TRIGGER, // A non movable body that triggers events when collided with -- to implement
		SURFACE, // Top down surface for applying friction
	};

	class CATWARE_API PhysicsBody
	{
	public:
		Vector2D position;
		void* userData;

		void ( *collisionCallback )( CollisionInfo info, PhysicsBody* body1, PhysicsBody* body2 ) = nullptr;

		PhysicsBody( BodyType bodyType );

		inline BodyType GetType( ) { return type; }

	private:
		BodyType type = BodyType::UNASSIGNED;
	};

	class CATWARE_API DynamicBody : public PhysicsBody
	{
	public:
		Vector2D velocity;
		Vector2D force;
		float mass;
		float restitution;

		DynamicBody( float mass, float restitution, Collider* collider );

		inline Collider* GetCollider( ) { return collider; }

	protected:
		Collider* collider;
	};

	class CATWARE_API SurfaceBody : public PhysicsBody
	{
	public:
		float frictionCoefficient = 0.9;

		SurfaceBody( float frictionCoefficient, Collider* collider );

	protected:
		Collider* collider;
	};

	class CATWARE_API PhysicsWorld
	{
	public:
		bool drawColliders = false;

		float gravity = 0;

		void AddBody( PhysicsBody* body );
		void RemoveBody( PhysicsBody* body );

		void Step( float deltaTime, unsigned int substeps = 8 );
		void DrawColliders( );

		void Clean( );
		void RemoveItems( );

	private:
		std::vector< PhysicsBody* > physicsBodies;
		std::unordered_set< PhysicsBody* > bodiesToRemove;
	};
}
