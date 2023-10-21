#include "Entity.h"

#include <random>

#include "CatWare/Application.h"

namespace CatWare
{
	namespace Entity
	{
		// ----------------------------------------
		// Entity ---------------------------------
		// ----------------------------------------
		void Entity::Destroy( )
		{
			shouldDelete = true;
		}


		void Entity::AddToGroup( std::string name )
		{
			groups.push_back( name );
		}

		void Entity::RemoveFromGroup( std::string name )
		{
			for ( std::vector<std::string>::iterator it; it != groups.end( ); it++ )
			{
				if ( ( *it ) == name )
				{
					groups.erase( it );
				}
			}
		}

		std::vector<std::string>& Entity::GetGroups( )
		{
			return groups;
		}


		std::string Entity::GetClassName( )
		{
			return className;
		}

		std::string Entity::GetUniqueName( )
		{
			return uniqueName;
		}

		UInt64 Entity::GetID( )
		{
			return id;
		}

		// ----------------------------------------
		// EntityRegistry -------------------------
		// ----------------------------------------
		std::unordered_map<std::string, Entity* ( * )( std::unordered_map<std::string, std::string> tags )> EntityRegistry::entityCreatePointers;

		Entity* ( *EntityRegistry::GetCreateFunction( std::string name ) )( std::unordered_map<std::string, std::string>tags )
		{
			return entityCreatePointers[name];
		}

		// ----------------------------------------
		// EntityManager --------------------------
		// ----------------------------------------
		void EntityManager::CleanUp( )
		{
			for ( Entity* entity : entities )
			{
				delete entity;
			}
		}
		
		UInt64 EntityManager::CreateEntityByClassName( std::string className, Transform transform, std::unordered_map<std::string, std::string> tags )
		{
			UInt64 id = std::rand( );

			if ( usedIDs[id] == false )
			{
				usedIDs[id] = true;

				Entity* entity = EntityRegistry::GetCreateFunction( className )( tags );
				entity->transform = transform;

				entities.push_back( entity );

				return id;
			}

			return 0;
		}

		void EntityManager::Update( )
		{
			for ( std::vector<Entity*>::iterator it = entities.begin( ); it != entities.end( ); )
			{
				Entity* entity = ( *it );

				if ( entity->shouldDelete )
				{
					entities.erase( it );
				}
				else
				{
					entity->Update( );

					entity->transform.position.x += entity->velocity.x * GetDeltaTime( );
					entity->transform.position.y += entity->velocity.y * GetDeltaTime( );

					it++;
				}
			}
		}

		void EntityManager::Draw( )
		{
			for ( Entity* entity : entities )
			{
				entity->Draw( );
			}
		}
	}
}