#include "Application.h"

#include <random>
#include <sstream>
#include <imgui.h>
#include "Debug/Debug.h"

#include "Log.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Text.h"
#include "Assets/Assets.h"
#include "Input/Binds.h"
#include "Random.h"

#include "SDL.h"

#include "CatWare/Filesystem/Filesystem.h"
#include "CatWare/Filesystem/Sources.h"

namespace CatWare
{
	void Application::Run( )
	{
		Init( );
		
		while ( running )
		{
			frameTimer.Reset( );

			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			const Scene* currentScene = SceneManager::GetCurrentScene( );

			if ( currentScene != nullptr )
			{
				Update( );

				int ticksElapsed = tickTimer.TimesTimeElapsed( ( 1.0 / Time::ticksPerSecond ) * Time::modifier );
			
				if ( ticksElapsed != 0 )
				{
					tickTimer.Reset( );
				}

				for ( ; ticksElapsed > 0; ticksElapsed-- )
				{
					Tick( );
				}

				Draw( );
				DrawGUI( );
			}

			Assets::textures.RunCleanup( );
			// Assets::sounds.RunCleanup( );


			while ( !frameTimer.HasTimeElapsed( 1 / Time::maxFPS ) && Time::frameRateLimited ) { }
			Time::SetDeltaTime( frameTimer.GetTime( ) );

			window->SwapBuffers( );
		}

		DeInit( );
	}

	void Application::Init( )
	{
		CatWare::Logging::InitLoggers( );

		CW_ENGINE_LOG->Info( "Initializing engine" );

		#ifdef CW_DEBUG
			CW_ENGINE_LOG->Warning( "Memory debugging enabled" )
		#endif

		CW_ENGINE_LOG->Info( "Initializing filesystem" );
		FileSystem::AddSource( new DirectorySource( "Custom" ) );
		FileSystem::AddSource( new DirectorySource( "." ) );

		Random::Init( );

		window = new Window( initConfig.windowTitle, initConfig.windowWidth, initConfig.windowHeight, initConfig.windowFullscreen );

		CW_ENGINE_LOG->Info( "Initializing renderer" );
		renderingAPI = new Rendering::OpenGL::OpenGLAPI;
		Renderer::Init( renderingAPI, initConfig.windowWidth, initConfig.windowHeight );

		// AudioEngine::InitAudio( );

		Text::InitFreetype( );

		PostInit( );

		frameTimer.Reset( );
		tickTimer.Reset( );
	}

	void Application::DeInit( )
	{
		PreDeInit( );
		
		Input::CleanUpBinds( );

		Renderer::DeInit( );
		// AudioEngine::DeInitAudio( );

		Assets::textures.Clear( );
		// Assets::sounds.Clear( );

		delete window;
		delete renderingAPI;

		#ifdef CW_DEBUG
		// Generate memory report
		std::stringstream memReport;

		std::vector<Debug::MemInfo> unfreedMemory = Debug::MemoryTracker::GetUnfreedMemory( );

		memReport << "[MEMORY REPORT]\n";

		if ( unfreedMemory.size( ) != 0 )
		{
			for ( Debug::MemInfo& mi : unfreedMemory )
			{
				memReport << "\tUnfreed memory at " << mi.address << " allocated in file " << mi.file << " at line " << mi.line << '\n';
			}
		}
		else
		{
			memReport << "No unfreed memory" << '\n';
		}

		CW_ENGINE_LOG->Info( memReport.str( ).c_str( ) );
		#endif
	}

	void Application::Update( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->Update( );
		currentScene->entityManager.Update( );
		currentScene->physicsWorld.Update( );

		// AudioEngine::UpdateHandles( );
	}

	void Application::Tick( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->Tick( );
		currentScene->entityManager.Tick( );

		currentScene->physicsWorld.Tick( );
	}

	void Application::Draw( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		Renderer::StartDrawing( );

		currentScene->Draw( );
		currentScene->entityManager.Draw( );

		Renderer::EndDrawing( );
	}

	void Application::DrawGUI( )
	{
		OrthoCamera* uiCamera = new OrthoCamera( window->GetWidth( ), window->GetHeight( ) );
		OrthoCamera* oldCamera = Renderer::camera2D;

		Renderer::camera2D = uiCamera;

		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->DrawGUI( );

		Renderer::camera2D = oldCamera;
		delete uiCamera;
	}
}