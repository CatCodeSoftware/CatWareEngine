#pragma once

#include "CatWare/Entity/Entity.h"
#include "CatWare/Physics/Physics.h"

namespace CatWare
{
	class World
	{
	public:
		EntityManager entities;
		PhysicsWorld physicsWorld;

		void Clean( );

		void LoadFromMapFile( const std::string& mapPath );
	};
}