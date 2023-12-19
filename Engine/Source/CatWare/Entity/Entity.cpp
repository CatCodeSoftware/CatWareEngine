#include "Entity.h"

#include "CatWare/Debug/Debug.h"

#include "CatWare/Application.h"
#include "CatWare/Log.h"
#include "CatWare/Random.h"

namespace CatWare
{
	void EntityCollisionCallback( PhysicsBody* body1, PhysicsBody* body2 )
	{
		if ( body1->userData != nullptr && body2->userData != nullptr )
		{
			( ( Entity* ) body1->userData )->OnCollide( ( Entity* ) body2->userData );
		}
	}

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
		int index = 0;
		for ( std::string nameInGroups : groups )
		{
			if ( name == nameInGroups )
			{
				groups.erase( groups.begin( ) + index );
				break;
			}

			index++;
		}
	}

	std::vector< std::string >& Entity::GetGroups( )
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
	std::unordered_map< std::string, Entity* ( * )( std::unordered_map< std::string, std::string > tags ) >
	EntityRegistry::entityCreatePointers;

	// ----------------------------------------
	// EntityManager --------------------------
	// ----------------------------------------
	EntityManager::EntityManager( )
	{
	}

	EntityManager::~EntityManager( )
	{
		CleanUp( );
	}

	void EntityManager::CleanUp( )
	{
		for ( Entity* entity : entities )
		{
			delete entity;
		}

		entities.clear( );
	}

	UInt64 EntityManager::CreateEntityByClassName(
		std::string className, Transform transform, std::unordered_map< std::string, std::string > tags
	)
	{
		UInt64 id = Random::GetUInt( 0, UINT64_MAX );

		if ( usedIDs[id] == false )
		{
			usedIDs[id] = true;

			auto createFunc = EntityRegistry::GetCreateFunction( className );

			if ( createFunc == nullptr )
				CW_ABORT( "createFunc was nullptr" );

			Entity* entity = createFunc( tags );

			entity->transform = transform;
			entity->id = id;

			entities.push_back( entity );

			entity->Init( );

			return id;
		}

		return 0;
	}


	Entity* EntityManager::GetEntityByID( UInt64 id )
	{
		for ( Entity* entity : entities )
		{
			if ( id == entity->id )
			{
				return entity;
			}
		}

		return nullptr;
	}

	Entity* EntityManager::GetEntityByUniqueName( const std::string& uniqueName )
	{
		for ( Entity* entity : entities )
		{
			if ( uniqueName == entity->uniqueName )
			{
				return entity;
			}
		}

		return nullptr;
	}

	std::vector< Entity* > EntityManager::GetEntitiesByClassName( const std::string& className )
	{
		std::vector< Entity* > gatheredEntities;

		for ( Entity* entity : entities )
		{
			if ( entity->className == className )
			{
				gatheredEntities.push_back( entity );
			}
		}

		return gatheredEntities;
	}

	std::vector< Entity* > EntityManager::GetEntitiesByGroup( const std::string& groupName )
	{
		std::vector< Entity* > gatheredEntities;

		for ( Entity* entity : entities )
		{
			// Todo: Find a better name for groupName2
			for ( const std::string& groupName2 : entity->groups )
			{
				if ( groupName == groupName2 )
				{
					gatheredEntities.push_back( entity );
					break;
				}
			}
		}

		return gatheredEntities;
	}


	void EntityManager::Update( )
	{
		for ( int i = 0; i < entities.size( ); i++ )
		{
			Entity* entity = entities[i];
			if ( entity->shouldDelete )
				entityDeleteQueue.push_back( entity );

			entity->Update( );
		}
	}

	void EntityManager::Tick( )
	{
		for ( Entity* entity : entities )
		{
			entity->Tick( );
		}
	}

	void EntityManager::Draw( )
	{
		for ( Entity* entity : entities )
		{
			entity->Draw( );
		}
	}

	void EntityManager::DestroyEntities( )
	{
		for ( Entity* entity : entityDeleteQueue )
		{
			delete entity;

			for ( auto it = entities.begin( ); it != entities.end( ); it++ )
			{
				if ( ( *it ) == entity )
				{
					entities.erase( it );
					break;
				}
			}
		}

		entityDeleteQueue.clear( );
	}
}
