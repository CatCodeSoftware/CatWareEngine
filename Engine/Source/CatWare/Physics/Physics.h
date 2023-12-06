#pragma once

#include <vector>

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
		SURFACE, // Top down surface for applying friction -- to implement
	};

	class CATWARE_API PhysicsBody
	{
	public:
		PhysicsBody( BodyType bodyType );

		inline BodyType GetType( ) { return type; }

		Vector2D position;

	private:
		BodyType type = BodyType::UNASSIGNED;
	};

	class CATWARE_API DynamicBody : public PhysicsBody
	{
	public:
		DynamicBody( float mass, Collider* collider );

		inline Collider* GetCollider( ) { return collider; }

		Vector2D velocity;
		Vector2D force;
		float mass;

	protected:
		Collider* collider;
	};


	class CATWARE_API PhysicsWorld
	{
	public:
		void AddBody( PhysicsBody* body );
		void RemoveBody( PhysicsBody* body );

		void Step( float deltaTime, unsigned int substeps = 4 );

	private:
		std::vector<PhysicsBody*> physicsBodies;
	};
}
