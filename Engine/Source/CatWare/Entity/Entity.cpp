#include "Entity.h"

namespace CatWare
{
	void EntityManager::Update( )
	{
		// AAAAAAAAAAAAAAAAAAAAAAA
		enttRegistry.view<EntityBehaviorComponent>( ).each( [=] ( auto entity, EntityBehaviorComponent& component )
		{
			if ( !component.behavior->initialized )
			{
				component.behavior->entity = Entity( this, ( UInt32 ) entity );
				component.behavior->Init( );
				component.behavior->initialized = true;
			}

			component.behavior->Update( );
		} );
	}

	void EntityManager::Tick( )
	{
		enttRegistry.view<EntityBehaviorComponent>( ).each( [=] ( auto entity, EntityBehaviorComponent& component )
		{
			component.behavior->Tick( );
		} );
	}

	void EntityManager::Draw( )
	{
		enttRegistry.view<EntityBehaviorComponent>( ).each( [=] ( auto entity, EntityBehaviorComponent& component )
		{
			component.behavior->Draw( );
		} );
	}

	Entity EntityManager::CreateEntity( std::string name, std::vector<std::string> groups )
	{
		UInt32 id = ( UInt32 ) enttRegistry.create( );

		return Entity( this, id );
	}

	Entity::Entity( )
	{
		this->manager = nullptr;
		this->id = 0;
	}

	Entity::Entity( EntityManager* manager, UInt32 id )
	{
		this->manager = manager;
		this->id = id;
	}
}