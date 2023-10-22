#pragma once

#include <vector>

#include "../Core.h"
#include "CatWare/Utils/Vector.h"
#include "../Entity/Entity.h"

namespace CatWare
{
	namespace Physics
	{
		class PhysicsObject
		{
		public:
			Entity::Transform* transform; // The transform to change when updating the object
			
			double mass;

			Vector2D velocity;
			Vector2D force;
		};

		class CATWARE_API PhysicsWorld
		{
		public:
			Vector2D gravity = { 0, 0 };

			void AddObject( PhysicsObject* physicsObject );
			void RemoveObject( PhysicsObject* physicsObject ); // Doesn't call delete! Beware of memory leaks.

			void Update( );

		private:
			std::vector<PhysicsObject*> physicsObjects;
		};
	}
}