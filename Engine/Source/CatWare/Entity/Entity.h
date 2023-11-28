#pragma once

#include <entt/entt.hpp>
#include <string>

#include "CatWare/Core.h"
#include "CatWare/Types/Types.h"
#include "CatWare/Types/Vector.h"
#include "CatWare/Log.h"

namespace CatWare
{
	class CATWARE_API EntityManager
	{
		friend class Entity;

	public:
		void Update( );
		void Tick( );
		void Draw( );

		Entity CreateEntity( std::string name, std::vector<std::string> groups );

	private:
		entt::registry enttRegistry;
	};

	class CATWARE_API Entity
	{
	public:
		Entity( );
		Entity( EntityManager* manager, UInt32 id );

		template<typename ComponentType, typename... Args>
		Entity& AddComponent( Args&&... args )
		{
			manager->enttRegistry.emplace<ComponentType>( ( entt::entity ) id, std::forward<Args>( args )... );
			return *this;
		}

		template<typename ComponentType>
		ComponentType* GetComponent( )
		{
			return &manager->enttRegistry.get<ComponentType>( ( entt::entity ) id );
		}

		template<typename ComponentType>
		bool HasComponent( )
		{
			return false;
		}

		template<typename ComponentType>
		void RemoveComponent( )
		{
			manager->enttRegistry.remove<ComponentType>( ( entt::entity ) id );
		}

	private:
		EntityManager* manager;
		UInt32 id;
	};

	class EntityBehavior
	{
		friend class EntityManager;
		friend class EntityBehaviorComponent;

	public:
		virtual void Init( ) { }
		virtual void DeInit( ) { }
		virtual void Update( ) { }
		virtual void Tick( ) { }
		virtual void Draw( ) { }

		template<typename ComponentType, typename... Args>
		void AddComponent( Args&&... args )
		{
			entity.AddComponent<ComponentType>( std::forward( args )... );
		}

		template<typename ComponentType>
		ComponentType* GetComponent( )
		{
			return entity.GetComponent<ComponentType>( );
		}

		template<typename ComponentType>
		void RemoveComponent( )
		{
			entity.RemoveComponent<ComponentType>( );
		}


	private:
		bool initialized = false;
		Entity entity;
	};

	class EntityBehaviorComponent
	{
	public:
		EntityBehaviorComponent( EntityBehavior* behavior )
		{
			this->behavior = behavior;
		}

		EntityBehavior* behavior;
	};
}