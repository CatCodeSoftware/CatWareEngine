#pragma once

#include <vector>

#include "CatWare/Types/Vector.h"

namespace CatWare
{
	enum class BodyType
	{
		UNASSIGNED, // Invalid body
		DYNAMIC, // A standard rigidbody
		TRIGGER, // A non movable body that triggers events when collided with -- to implement
		SURFACE, // Top down surface for applying friction -- to implement
	};

	class PhysicsBody
	{
	public:
		PhysicsBody( BodyType bodyType );

		inline BodyType GetType( ) { return type; }

		Vector2D position;

	private:
		BodyType type = BodyType::UNASSIGNED;
	};

	class DynamicBody : public PhysicsBody
	{
	public:
		DynamicBody( float mass );

		Vector2D velocity;
		Vector2D force;
		float mass;
	};


	class PhysicsWorld
	{
	public:
		void Step( float deltaTime, unsigned int substeps = 4 );

	private:
		std::vector<PhysicsBody*> physicsBodies;
	};
}
