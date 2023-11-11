/**
* @file Physics.h
* 
* @brief The physics module
* 
* @author PointThink
*/

#pragma once

#include <vector>

#include "../Core.h"
#include "CatWare/Utils/Vector.h"
#include "CatWare/Utils/Transform.h"
#include "Collision.h"
#include "CollisionResponse.h"

namespace CatWare
{
	namespace Physics
	{
		/** 
		* A class to represent a single physics object in the world
		* 
		* Not really meant to be used standalone but bound to an entity.
		* Look up CatWare::Entity::AttachPhysicsObject
		*/
		class CATWARE_API PhysicsObject
		{
		public:
			void AttachCollider( Collider* collider, Vector2D relativePos = { 0, 0 } );
			void DetachCollider( Collider* collider );

			inline std::vector<Collider*> GetColliders( ) { return colliders; }

			void SetResponse( CollisionResponse* response );

			//! A pointer to the transform to modify when the object is updated
			Transform* transform;
			
			double mass = 1;

			bool frictionEnabled = false;
			double frictionCoefficient = 1.0;

			Vector2D velocity;
			Vector2D force;

			double resistance = 1;

			bool movable = false;
			bool collidable = true;

		private:
			std::vector<Collider*> colliders;
			CollisionResponse* response = nullptr;
		};


		//! Controls all physics objects
		class CATWARE_API PhysicsWorld
		{
		public:
			Vector2D gravity = { 0, 0 };

			void AddObject( PhysicsObject* physicsObject );
			void RemoveObject( PhysicsObject* physicsObject ); // Doesn't call delete! Beware of memory leaks.

			void Tick( );
			void Update( );

		private:
			void ResolveCollisions( PhysicsObject* physObj1, PhysicsObject* physObj2 );

			std::vector<PhysicsObject*> physicsObjects;
		};
	}
}