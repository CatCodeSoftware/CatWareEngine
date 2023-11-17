#pragma once

#include <vector>
#include <array>

#include <box2d/box2d.h>

#include "CatWare/Core.h"
#include "CatWare/Utils/Transform.h"
#include "CatWare/Utils/Vector.h"

namespace CatWare
{
	enum class ShapeType
	{
		POLYGON, CIRCLE
	};

	class Shape
	{
	public:
		virtual ShapeType GetShapeType( ) = 0;
		virtual b2Shape* GetB2Shape( ) = 0;
	};

	class CATWARE_API PolygonShape : public Shape
	{
	public:
		inline ShapeType GetShapeType( ) override { return ShapeType::POLYGON; }
		inline b2Shape* GetB2Shape( ) override { return &shape; }

		void SetAsRect( Vector2D size );
		void SetVerts( int count, Vector2D* verts );

	private:
		b2PolygonShape shape;
	};

	class CATWARE_API PhysicsObject
	{
	public:
		//! Pointer to transform to modify
		Transform* transform;
		b2Body* body;
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
		std::vector<PhysicsObject*> objects;
		b2World* world;
	};
}