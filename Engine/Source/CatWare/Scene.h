#pragma once

#include "Core.h"
#include "Entity/Entity.h"
#include "Physics/Physics.h"

namespace CatWare
{
	class Scene
	{
		friend class Application;

	public:
		// called when the currentScene is set to this
		virtual void OnEnter( ) {}

		// called when currentScene is set to something diffrent
		virtual void OnExit( ) {}

		virtual void Update( ) {}
		virtual void Tick( ) {}
		virtual void Draw( ) {}
		virtual void DrawGUI( ) {} // Ignores the render offset

		EntityManager entityManager;
		Physics::PhysicsWorld physicsWorld;
	};

	class CATWARE_API SceneManager
	{
	public:
		static void SetScene( Scene* scene );
		static Scene* GetCurrentScene( );

	private:
		static Scene* currentScene;
	};
}