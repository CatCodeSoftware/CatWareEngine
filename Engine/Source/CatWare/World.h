#pragma once

#include "Particles.h"
#include "CatWare/Entity/Entity.h"
#include "CatWare/Physics/Physics.h"

namespace CatWare
{
	class World
	{
	public:
		EntityManager entities;
		PhysicsWorld physicsWorld;
		Particles particles;

		void Clean( );

		void LoadFromMapFile( const std::string& mapPath );
	};
}