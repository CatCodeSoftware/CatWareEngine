#pragma once

#include <vector>

#include "../Core.h"
#include "CatWare/Utils/Vector.h"
#include "CatWare/Utils/Transform.h"
#include "Collision.h"

namespace CatWare
{
	namespace Physics
	{
		class PhysicsObject
		{
		public:
			Transform* transform; // The transform to change when updating the object
			
			double mass = 1;

			bool frictionEnabled = false;
			double frictionCoefficient = 1.0;

			Vector2D velocity;
			Vector2D force;

			std::vector<Collider*> colliders;
		};


		// Controlls all physics objects
		class CATWARE_API PhysicsWorld
		{
		public:
			Vector2D gravity = { 0, 0 };

			void AddObject( PhysicsObject* physicsObject );
			void RemoveObject( PhysicsObject* physicsObject ); // Doesn't call delete! Beware of memory leaks.

			void Tick( );
			void Update( );

		private:
			std::vector<PhysicsObject*> physicsObjects;
		};
	}
}