#include "Scene.h"

#include "Error.h"

namespace CatWare
{
	Scene* SceneManager::currentScene = nullptr;

	void SceneManager::SetScene( Scene* scene )
	{
		if ( currentScene != nullptr )
			currentScene->OnExit( );

		if ( scene == nullptr )
			CW_ABORT( "Attemped to set scene that doesn't exist" );

		currentScene = scene;
		scene->OnEnter( );
	}

	Scene* SceneManager::GetCurrentScene( )
	{
		return currentScene;
	}
}