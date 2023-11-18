#pragma once

#include <vector>
#include <array>

#include <box2d/box2d.h>

#include "CatWare/Core.h"
#include "CatWare/Utils/Transform.h"
#include "CatWare/Utils/Vector.h"

namespace CatWare
{
	class Entity;

	class Shape
	{
	public:
		virtual b2Shape* GetB2Shape( ) = 0;
	};

	class CATWARE_API PolygonShape : public Shape
	{
	public:
		inline b2Shape* GetB2Shape( ) override { return &shape; }

		void SetAsRect( Vector2D size );
		void SetVerts( int count, Vector2D* verts );

	private:
		b2PolygonShape shape;
	};

	class CATWARE_API CircleShape : public Shape
	{
	public:
		CircleShape( float radius );

		inline b2Shape* GetB2Shape( ) override { return &shape; }

	private:
		b2CircleShape shape;
	};

	class CollisionCallback : public b2ContactListener
	{
	public:
		void BeginContact( b2Contact* contact ) override;
		void EndContact( b2Contact* contact ) override;
	};

	class CATWARE_API PhysicsObject
	{
	public:
		~PhysicsObject( );

		//! Pointer to transform to modify
		Transform* transform = nullptr;
		Entity* attachedEntity = nullptr;
		b2Body* body = nullptr;

		void ( *onCollideBegin )( PhysicsObject* object1, PhysicsObject* object2 );
		void ( *onCollideEnd )( PhysicsObject* object1, PhysicsObject* object2 );

		Vector2D GetVelocity( );
		float GetAngularVelocity( );
		bool GetFixedRotation( );
		Vector2D GetWorldCenter( );
		
		void SetVelocity( Vector2D velocity );
		void SetAngularVelocity( float angularVelocity );
		void SetFixedRotation( bool fixed );

		void ApplyForce( Vector2D force, Vector2D point );
		void ApplyImpulse( Vector2D force, Vector2D point );
	};

	class CATWARE_API PhysicsWorld
	{
	public:
		PhysicsWorld( );
		~PhysicsWorld( );

		void Update( );
		void Tick( );

		PhysicsObject* CreateObject( Transform* transform, Shape* shape, bool dynamic, float density, float friction );
		void RemoveObject( PhysicsObject* object );

		void SetGravity( Vector2D gravity );
		Vector2D GetGravity( );

	private:
		CollisionCallback* collisionCallback;
		std::vector<PhysicsObject*> objects;
		b2World* world;
	};

}