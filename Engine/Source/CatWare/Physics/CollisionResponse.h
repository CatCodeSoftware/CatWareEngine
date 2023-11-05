#pragma once

#include "Collision.h"

namespace CatWare
{
    namespace Physics
    {
        class PhysicsObject;

        class CollisionResponse
        {
        public:
            virtual void OnCollide( PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo collisionInfo ) = 0;
        };
    }
}