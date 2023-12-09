#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "CatWare/Core.h"
#include "CatWare/Error.h"
#include "CatWare/Types/Vector.h"
#include "CatWare/Types/Types.h"
#include "CatWare/Types/Transform.h"

#undef GetClassName // stupid windows

namespace CatWare
{
	// This is a base class for all entities in the game
	class CATWARE_API Entity
	{
		friend class EntityManager;

	public:
		Transform transform;
		Vector2D velocity = { 0, 0 };

		virtual void Init( ) { }
		virtual void Update( ) { }
		virtual void Tick( ) { }
		virtual void Draw( ) { }

		void Destroy( );

		void AddToGroup( std::string name );
		void RemoveFromGroup( std::string name );
		std::vector<std::string>& GetGroups( );

		std::string GetClassName( );
		std::string GetUniqueName( );

		UInt64 GetID( );

		static Entity* Create( std::unordered_map<std::string, std::string> tags ) { return nullptr; }

	protected:
		std::string className;
		std::string uniqueName;

		std::vector<std::string> groups;
	private:
		UInt64 id = 0;

		bool shouldDelete = false;
	};

	// This static class holds info about how to create entities
	class CATWARE_API EntityRegistry
	{
	public:
		template<typename T>
		static void RegisterEntity( std::string name )
		{
			if ( &( T::Create ) == nullptr )
				CW_ABORT( "Entity does not have a create function" );

			EntityRegistry::entityCreatePointers.insert( { name, &( T::Create ) } );
		}

		static Entity* ( *GetCreateFunction( std::string name ) )( std::unordered_map<std::string, std::string> tags )
		{
			return EntityRegistry::entityCreatePointers[name];
		}

	private:
		// stores pointers to a create function
		static std::unordered_map<std::string, Entity* ( * )( std::unordered_map<std::string, std::string> tags )> entityCreatePointers;
	};

	// This is a scene specific class that manages all entities
	class CATWARE_API EntityManager
	{
	public:
		EntityManager( );
		~EntityManager( );

		void CleanUp( );

		// Function for creating an entity by type
		template<typename T>
		UInt64 CreateEntityByType( Transform transform, std::unordered_map<std::string, std::string> tags )
		{
			UInt64 id = std::rand( );

			Entity* entity = T::Create( tags );
			entity->transform = transform;

			entity->id = id;

			if ( usedIDs[id] == false )
			{
				usedIDs[id] = true;

				entities.push_back( entity );
				return id;
			}

			return 0;
		}

		UInt64 CreateEntityByClassName( std::string className, Transform transform, std::unordered_map<std::string, std::string> tags );

		Entity* GetEntityByID( UInt64 id );
		Entity* GetEntityByUniqueName( const std::string &uniqueName );
		std::vector<Entity*> GetEntitiesByClassName( const std::string &className );
		std::vector<Entity*> GetEntitiesByGroup( const std::string &groupName );

		void DestroyEntities( );

		void Update( );
		void Tick( );
		void Draw( );
	private:
		std::unordered_map<UInt64, bool> usedIDs;
		std::vector<Entity*> entities;
		std::vector<Entity*> entityDeleteQueue;
	};
}