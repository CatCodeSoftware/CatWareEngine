/**	
* @file Scene.h
* 
* @brief Manages the game scenes
* 
* @author PointThink
*/

#pragma once

#include "Core.h"

#include "World.h"

namespace CatWare
{
	/**
	* The Scene class is meant to be inherited from by another class
	* which represents the current game state
	*/
	class Scene
	{
		friend class Application;

	public:
		World world;

		// called when the currentScene is set to this
		virtual void OnEnter( ) {}

		// called when currentScene is set to something diffrent
		virtual void OnExit( ) {}

		virtual void Update( ) {}
		virtual void Tick( ) {}
		virtual void Draw( ) {}
		virtual void DrawGUI( ) {} // Ignores the render offset
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