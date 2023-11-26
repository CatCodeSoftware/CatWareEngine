#include "Entity.h"

#include "Components.h"


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
		auto group = enttRegistry.group<Transform>( entt::get<SpriteRenderer> );

		for ( auto entity : group )
		{
			auto [transform, rectRenderer] = group.get<Transform, SpriteRenderer>( entity );

			rectRenderer.Draw( transform );
		}

		enttRegistry.view<EntityBehaviorComponent>( ).each( [=] ( auto entity, EntityBehaviorComponent& component )
		{
			component.behavior->Draw( );
		} );
	}

	Entity EntityManager::CreateEntity( std::string name, std::vector<std::string> groups )
	{
		UInt32 id = ( UInt32 ) enttRegistry.create( );
		Entity entity( this, id );

		// entity.AddComponent<BasicEntityInfo>( name, groups );

		return entity;
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