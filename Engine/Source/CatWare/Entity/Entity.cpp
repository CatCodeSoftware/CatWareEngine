#include "Entity.h"

#include <random>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Application.h"
#include "CatWare/Utils/Log.h"

namespace CatWare
{
	void CollisionBegin( PhysicsObject* object1, PhysicsObject* object2 )
	{
		if ( object1->attachedEntity != nullptr && object2->attachedEntity != nullptr )
		{
			object1->attachedEntity->OnCollisionBegin( object2->attachedEntity );
			object2->attachedEntity->OnCollisionBegin( object1->attachedEntity );
		}
	}

	void CollisionEnd( PhysicsObject* object1, PhysicsObject* object2 )
	{
		if ( object1->attachedEntity != nullptr && object2->attachedEntity != nullptr )
		{
			object1->attachedEntity->OnCollisionEnd( object2->attachedEntity );
			object2->attachedEntity->OnCollisionEnd( object1->attachedEntity );
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
		for ( std::vector<std::string>::iterator it; it != groups.end( ); it++ )
		{
			if ( ( *it ) == name )
			{
				groups.erase( it );
				break;
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


	PhysicsObject* Entity::AttachPhysicsObject( Shape* shape, bool dynamic, float density, float friction )
	{
		if ( attachedPhysicsObject != nullptr )
		{
			CW_ENGINE_LOG->Warning( "Attempted to attach a physics object to an entity that already has one" );
			return attachedPhysicsObject;
		}

		PhysicsWorld* physicsWorld = &SceneManager::GetCurrentScene( )->physicsWorld;

		attachedPhysicsObject = physicsWorld->CreateObject( &transform, shape, dynamic, density, friction );
		attachedPhysicsObject->attachedEntity = this;
		attachedPhysicsObject->onCollideBegin = &CollisionBegin;
		attachedPhysicsObject->onCollideEnd = &CollisionEnd;
		
		return attachedPhysicsObject;
	}

	PhysicsObject* Entity::GetAttachedPhysicsObject( )
	{
		return attachedPhysicsObject;
	}

	void Entity::DetachPhysicsObject( )
	{
		if ( attachedPhysicsObject == nullptr )
		{
			CW_ENGINE_LOG->Warning( "Attempted to detach a physics object from entity which doesn't have one" );
			return;
		}

		SceneManager::GetCurrentScene( )->physicsWorld.RemoveObject( attachedPhysicsObject );

		delete attachedPhysicsObject;
		attachedPhysicsObject = nullptr;
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
	}
		
	UInt64 EntityManager::CreateEntityByClassName( std::string className, Transform transform, std::unordered_map<std::string, std::string> tags )
	{
		UInt64 id = std::rand( );

		if ( usedIDs[id] == false )
		{
			usedIDs[id] = true;

			Entity* entity = EntityRegistry::GetCreateFunction( className )( tags );
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

	Entity* EntityManager::GetEntityByUniqueName( std::string uniqueName )
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

	std::vector<Entity*> EntityManager::GetEntitiesByClassName( std::string className )
	{
		std::vector<Entity*> gatheredEntities;

		for ( Entity* entity : entities )
		{
			if ( entity->className == className )
			{
				gatheredEntities.push_back( entity );
			}
		}

		return gatheredEntities;
	}

	std::vector<Entity*> EntityManager::GetEntitiesByGroup( std::string groupName )
	{
		std::vector<Entity*> gatheredEntities;

		for ( Entity* entity : entities )
		{
			// Todo: Find a better name for groupName2
			for ( std::string groupName2 : entity->groups )
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
		std::vector<std::vector<Entity*>::iterator> entityDeleteQueue;

		for ( std::vector<Entity*>::iterator it = entities.begin( ); it != entities.end( ); it++ )
		{
			Entity* entity = ( *it );

			if ( entity->shouldDelete )
			{
				entityDeleteQueue.push_back( it );
			}
			else
			{
				entity->Update( );
			}
		}

		for ( auto it : entityDeleteQueue )
		{
			delete *it;
			entities.erase( it );
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
}