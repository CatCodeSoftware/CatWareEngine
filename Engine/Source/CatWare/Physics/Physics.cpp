#include "Physics.h"

#include "..\Utils\Time.h"

namespace CatWare
{
	b2Vec2 VecToB2Vec( Vector2D vec )
	{
		return b2Vec2( vec.x, vec.y );
	}

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

	CircleShape::CircleShape( float radius )
	{
		shape.m_radius = radius;
	}

	// Object
	PhysicsObject::~PhysicsObject( )
	{
		body->GetWorld( )->DestroyBody( body );
	}

	void PhysicsObject::SetFixedRotation( bool fixed )
	{
		body->SetFixedRotation( true );
	}

	void PhysicsObject::SetVelocity( Vector2D velocity )
	{
		body->SetLinearVelocity( VecToB2Vec( velocity ) );
	}

	void PhysicsObject::SetAngularVelocity( float velocity )
	{
		body->SetAngularVelocity( velocity );
	}


	Vector2D PhysicsObject::GetVelocity( )
	{
		b2Vec2 vec = body->GetLinearVelocity( );
		return { vec.x, vec.y };
	}

	float PhysicsObject::GetAngularVelocity( )
	{
		return body->GetAngularVelocity( );
	}

	bool PhysicsObject::GetFixedRotation( )
	{
		return body->IsFixedRotation( );
	}

	Vector2D PhysicsObject::GetWorldCenter( )
	{
		b2Vec2 vec = body->GetWorldCenter( );
		return { vec.x, vec.y };
	}


	void PhysicsObject::ApplyImpulse( Vector2D force, Vector2D point )
	{
		body->ApplyLinearImpulse( VecToB2Vec( force ), VecToB2Vec( point ), true );
	}

	void PhysicsObject::ApplyForce( Vector2D force, Vector2D point )
	{
		body->ApplyForce( VecToB2Vec( force ), VecToB2Vec( point ), true );
	}
	

	// World
	PhysicsWorld::PhysicsWorld( )
	{
		b2Vec2 gravity( 0, 0 );
		world = new b2World( gravity );

		collisionCallback = new CollisionCallback;
		world->SetContactListener( collisionCallback );
	}

	PhysicsWorld::~PhysicsWorld( )
	{
		delete collisionCallback;
		delete world;
	}

	void PhysicsWorld::Update( )
	{
		for ( PhysicsObject* object : objects )
		{
			object->transform->position = object->transform->position + object->GetVelocity( ) * Vector2D( GlobalTime::GetDeltaTime( ), GlobalTime::GetDeltaTime( ) );
			object->transform->rotation += object->GetAngularVelocity( ) * GlobalTime::GetDeltaTime( );
		}
	}

	void PhysicsWorld::Tick( )
	{
		world->Step( 1.0 / GlobalTime::ticksPerSecond, 1, 1 );

		for ( PhysicsObject* object : objects )
		{
			b2Vec2 pos = object->body->GetPosition( );
			object->transform->position = { pos.x, pos.y };
			object->transform->rotation = object->body->GetAngle( ) * 57.2957795;

			b2Vec2 velocity = object->body->GetLinearVelocity( );
			float angularVelocity = object->body->GetAngularVelocity( );

			if ( velocity == b2Vec2( 0, 0 ) && round( angularVelocity ) == 0 )
			{
				object->body->SetAwake( 0 );
			}
		}
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

		object->body->GetUserData( ).pointer = ( uintptr_t ) object;

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

	// CollisionCallback
	void CollisionCallback::BeginContact( b2Contact* contact )
	{
		PhysicsObject* object1 = ( ( PhysicsObject* )contact->GetFixtureA( )->GetBody( )->GetUserData( ).pointer );
		PhysicsObject* object2 = ( ( PhysicsObject* )contact->GetFixtureB( )->GetBody( )->GetUserData( ).pointer );

		if ( object1->onCollideBegin != nullptr ) object1->onCollideBegin( object1, object2 );
		if ( object2->onCollideBegin != nullptr ) object2->onCollideBegin( object2, object1 );
	}

	void CollisionCallback::EndContact( b2Contact* contact )
	{
		PhysicsObject* object1 = ( ( PhysicsObject* )contact->GetFixtureA( )->GetBody( )->GetUserData( ).pointer );
		PhysicsObject* object2 = ( ( PhysicsObject* )contact->GetFixtureB( )->GetBody( )->GetUserData( ).pointer );

		if ( object1->onCollideEnd != nullptr ) object1->onCollideEnd( object1, object2 );
		if ( object2->onCollideEnd != nullptr ) object2->onCollideEnd( object2, object1 );
	}
}