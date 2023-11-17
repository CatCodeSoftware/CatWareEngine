#include "Physics.h"

#include "..\Utils\Time.h"

namespace CatWare
{
	// Shapes
	void PolygonShape::SetAsRect( Vector2D size )
	{
		shape.SetAsBox( size.x / 2, size.y / 2 );
	}

	void PolygonShape::SetVerts( int count, Vector2D* verts )
	{
		// convert to b2vec2
		b2Vec2* vecs = new b2Vec2[count];

		for ( unsigned int i = 0; i < count; i++ )
		{
			vecs[i].Set( verts[i].x, verts[i].y );
		}

		shape.Set( vecs, count );
	}

	// World
	PhysicsWorld::PhysicsWorld( )
	{
		b2Vec2 gravity( 0, 0 );
		world = new b2World( gravity );
	}

	PhysicsWorld::~PhysicsWorld( )
	{
		delete world;
	}

	void PhysicsWorld::Update( )
	{
		world->Step( GlobalTime::GetDeltaTime( ), 8, 3 );

		for ( PhysicsObject* object : objects )
		{
			b2Vec2 pos = object->body->GetPosition( );
			object->transform->position = { pos.x, pos.y };
			object->transform->rotation = object->body->GetAngle( ) * 57.2957795;
		}
	}

	void PhysicsWorld::Tick( )
	{

	}

	PhysicsObject* PhysicsWorld::CreateObject( Transform* transform, Shape* shape, bool dynamic, float density, float friction )
	{
		PhysicsObject* object = new PhysicsObject( );

		object->transform = transform;

		b2BodyDef bodyDef;

		if ( dynamic ) bodyDef.type = b2_dynamicBody;
		else bodyDef.type = b2_staticBody;

		bodyDef.position.Set( transform->position.x, transform->position.y );


		b2FixtureDef fixtureDef;

		fixtureDef.shape = shape->GetB2Shape( );
		fixtureDef.density = density;
		fixtureDef.friction = friction;

		object->body = world->CreateBody( &bodyDef );
		object->body->CreateFixture( &fixtureDef );

		objects.push_back( object );

		return object;
	}

	void PhysicsWorld::RemoveObject( PhysicsObject* object )
	{
		for ( std::vector<PhysicsObject*>::iterator it = objects.begin( ); it != objects.end( ); )
		{
			if ( *it == object )
			{
				objects.erase( it );
				delete ( *it );

				break;
			}
			else
			{
				it++;
			}
		}
	}

	void PhysicsWorld::SetGravity( Vector2D gravity )
	{
		b2Vec2 vec = b2Vec2( gravity.x, gravity.y );
		world->SetGravity( vec );
	}

	Vector2D PhysicsWorld::GetGravity( )
	{
		b2Vec2 gravity = world->GetGravity( );
		return { gravity.x, gravity.y };
	}
}