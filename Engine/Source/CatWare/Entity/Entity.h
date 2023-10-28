#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "CatWare/Core.h"
#include "CatWare/Utils/Vector.h"
#include "CatWare/Utils/Types.h"

namespace CatWare
{
	class Transform
	{
	public:
		Vector2D position = { 0, 0 };
		Vector2D size = { 0, 0 };
			
		Vector2D rotationPoint = { 0, 0 };
		float rotation = 0;
	};

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

		static Entity* Create( std::unordered_map<std::string, std::string> tags ) { return nullptr;  }

	protected:
		std::string className;
		std::string uniqueName;
			
		std::vector<std::string> groups;
	private:
		UInt64 id;

		bool shouldDelete = false;
	};

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

	class CATWARE_API EntityManager
	{
	public:
		~EntityManager( ) {}

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

		void Update( );
		void Draw( );
	private:
		std::unordered_map<UInt64, bool> usedIDs;
		std::vector<Entity*> entities;
	};
}