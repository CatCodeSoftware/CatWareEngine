#include "Entity.h"

#include <random>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Application.h"
#include "CatWare/Utils/Log.h"

namespace CatWare
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


	void Entity::AttachPhysicsObject( double mass, bool frictionEnabled, double frictionCoefficient, std::vector<Physics::Collider*> colliders )
	{
		if ( attachedPhysicsObject != nullptr )
		{
			CW_ENGINE_LOG->Warning( "Attempted to attach a physics object to entity that already has one" );
			return;
		}

		attachedPhysicsObject = new Physics::PhysicsObject;

		attachedPhysicsObject->transform = &transform;

		attachedPhysicsObject->mass = mass;
		attachedPhysicsObject->frictionEnabled = frictionEnabled;
		attachedPhysicsObject->frictionCoefficient = frictionCoefficient;
		attachedPhysicsObject->colliders = colliders;

		SceneManager::GetCurrentScene( )->physicsWorld.AddObject( attachedPhysicsObject );
	}

	Physics::PhysicsObject* Entity::GetAttachedPhysicsObject( )
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

		for ( Physics::Collider* collider : attachedPhysicsObject->colliders )
		{
			delete collider;
		}

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

				it++;
			}
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