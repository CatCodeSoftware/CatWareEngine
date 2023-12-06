#include "Physics.h"

namespace CatWare
{
	PhysicsBody::PhysicsBody( BodyType bodyType )
	{
		this->type = bodyType;
	}

	DynamicBody::DynamicBody( float mass ) : PhysicsBody( BodyType::DYNAMIC )
	{
		this->mass = mass;
	}

	void PhysicsWorld::Step( float deltaTime, unsigned substeps )
	{
		for ( PhysicsBody* physicsBody : physicsBodies )
		{
			if ( physicsBody->GetType( ) == BodyType::DYNAMIC )
			{
				DynamicBody* dynamicBody = ( DynamicBody* ) physicsBody;

				dynamicBody->force = { 0, 0 };
			}
		}
	}
}