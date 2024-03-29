#include "Entity.h"

#include <algorithm>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Application.h"
#include "CatWare/Log.h"
#include "CatWare/Random.h"

namespace CatWare
{
	class LessThanKey
	{
	public:
		inline bool operator() (const Entity* struct1, const Entity* struct2)
		{
			return (struct1->transform.layer < struct2->transform.layer);
		}
	};

	void EntityCollisionCallback( CollisionInfo info, PhysicsBody* body1, PhysicsBody* body2 )
	{
		if ( body1->userData != nullptr && body2->userData != nullptr )
		{
			( ( Entity* ) body1->userData )->OnCollide( info, body2, ( Entity* ) body2->userData );
		}
	}

	// ----------------------------------------
	// Entity ---------------------------------
	// ----------------------------------------
	void Entity::Destroy( )
	{
		shouldDelete = true;
	}


	void Entity::AttachPhysicsBody( PhysicsBody* physicsBody )
	{
		this->physicsBody = physicsBody;
		physicsBody->position = transform.position;
		physicsBody->userData = this;
		physicsBody->collisionCallback = &EntityCollisionCallback;
		CW_CURRENT_SCENE->world.physicsWorld.AddBody( physicsBody );
	}

	void Entity::DetachPhysicsBody( )
	{
		CW_CURRENT_SCENE->world.physicsWorld.RemoveBody( physicsBody );
		physicsBody = nullptr;
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

	bool Entity::BelongsToGroup( std::string group )
	{
		for ( std::string& group2 : groups )
		{
			if ( group == group2 )
				return true;
		}

		return false;
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

	UInt64 EntityManager::CreateEntity(
		std::string className, Transform transform, std::unordered_map< std::string, std::string > tags )
	{
		UInt64 id = Random::GetUInt( 0, UINT64_MAX );

		if ( usedIDs[id] == false )
		{
			usedIDs[id] = true;

			auto createFunc = EntityRegistry::GetCreateFunction( className );

			if ( createFunc == nullptr )
				CW_ABORT( "createFunc for " + className + " was nullptr" );

			Entity* entity = createFunc( tags );

			entity->transform = transform;
			entity->id = id;
			entity->className = className;

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

			if ( entity->physicsBody != nullptr )
				entity->transform.position = entity->physicsBody->position;
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
		std::vector<Entity*> sortedList = entities;
		std::sort( sortedList.begin( ), sortedList.end( ), LessThanKey( ) );

		for ( Entity* entity : sortedList )
		{
			entity->Draw( );
		}
	}

	void EntityManager::DestroyEntities( )
	{
		for ( Entity* entity : entityDeleteQueue )
		{
			if ( entity->physicsBody != nullptr )
				entity->DetachPhysicsBody( );

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
