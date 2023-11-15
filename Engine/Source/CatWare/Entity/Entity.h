#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "CatWare/Core.h"
#include "CatWare/Utils/Vector.h"
#include "CatWare/Utils/Types.h"
#include "CatWare/Utils/Transform.h"
#include "CatWare/Physics/Physics.h"
#include "CatWare/Physics/Collision.h"

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

		virtual void Init( ) {}
		virtual void Update( ) {}
		virtual void Tick( ) {}
		virtual void Draw( ) {}

		void Destroy( );

		void AddToGroup( std::string name );
		void RemoveFromGroup( std::string name );
		std::vector<std::string>& GetGroups( );

		std::string GetClassName( );
		std::string GetUniqueName( );

		UInt64 GetID( );

		// Physics object stuff
		Physics::PhysicsObject* AttachPhysicsObject( double mass = 1.0, bool frictionEnabled = false, double frictionCoefficient = 1 );
		Physics::PhysicsObject* GetAttachedPhysicsObject( );
		void DetachPhysicsObject( );

		static Entity* Create( std::unordered_map<std::string, std::string> tags ) { return nullptr;  }

	protected:
		std::string className;
		std::string uniqueName;
			
		std::vector<std::string> groups;
	private:
		UInt64 id;

		bool shouldDelete = false;

		Physics::PhysicsObject* attachedPhysicsObject = nullptr;
	};

    // This static class holds info about how to create entities
	class CATWARE_API EntityRegistry
	{
	public:
		template<typename T>
		static void RegisterEntity( std::string name )
		{
			entityCreatePointers[name] = &(T::Create);
		}

		static Entity* ( *GetCreateFunction( std::string name ) )( std::unordered_map<std::string, std::string> tags );

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
		Entity* GetEntityByUniqueName( std::string uniqueName );
		std::vector<Entity*> GetEntitiesByClassName( std::string name );
		std::vector<Entity*> GetEntitiesByGroup( std::string groupName );

		void Update( );
		void Tick( );
		void Draw( );
	private:
		std::unordered_map<UInt64, bool> usedIDs;
		std::vector<Entity*> entities;
	};
}