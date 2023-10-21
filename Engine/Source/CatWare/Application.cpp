#include "Application.h"

#include <stb_image.h>
#include <random>

#include "Utils/Color.h"
#include "Graphics/Renderer/Renderer.h"
#include "Resource/TextureResource.h"
#include "Error.h"

namespace CatWare
{
	static double deltaTime;

	double GetDeltaTime( )
	{
		return deltaTime;
	}

	using namespace CatWare::Rendering;

	Application::Application( )
	{

	}

	Application::~Application( )
	{

	}

	void Application::Run( )
	{
		// temporary srand call - PT
		std::srand( time( NULL ) );

		window = new Window( initConfig.windowTitle, initConfig.windowWidth, initConfig.windowHeight, initConfig.windowFullscreen );

		Rendering::Renderer::SetScreenSize( window->GetWidth( ), window->GetHeight( ) );
		Renderer::Init( new OpenGL::OpenGLAPI );

		PostInit( );

		frameTimer.Reset( );

		while ( running )
		{
			deltaTime = frameTimer.GetTime( );
			frameTimer.Reset( );

			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			currentScene->Update( );
			currentScene->entityManager.Update( );

			Renderer::StartDrawing( );

			currentScene->Draw( );
			currentScene->entityManager.Draw( );

			Renderer::EndDrawing( );

			window->SwapBuffers( );
		}

		TextureManager::RemoveEverything( );

		delete window;
	}

	void Application::SetScene( Scene* scene )
	{
		if ( currentScene != nullptr )
			currentScene->SwitchOff( );

		if ( scene == nullptr )
			CW_ABORT( "Attemped to set scene that doesn't exist" );

		this->currentScene = scene;
		scene->SwitchTo( );
	}
}