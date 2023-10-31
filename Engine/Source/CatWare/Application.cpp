#include "Application.h"

#include <random>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "Utils/Log.h"
#include "Utils/Color.h"
#include "Graphics/Renderer/Renderer.h"
#include "Resource/TextureResource.h"
#include "Error.h"
#include "Graphics/Text.h"

namespace CatWare
{
	Application::Application( )
	{

	}

	Application::~Application( )
	{

	}

	void Application::Run( )
	{
		Init( );

		while ( running )
		{
			GlobalTime::SetDeltaTime( frameTimer.GetTime( ) );
			frameTimer.Reset( );

			int ticksElapsed = tickTimer.TimesTimeElapsed( ( 1.0 / ticksPerSecond ) * GlobalTime::modifier );
			
			if ( ticksElapsed != 0 )
			{
				tickTimer.Reset( );
			}

			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			Scene* currentScene = SceneManager::GetCurrentScene( );

			if ( currentScene != nullptr )
			{
				Update( );

				for ( ; ticksElapsed > 0; ticksElapsed-- )
				{
					Tick( );
				}

				Draw( );
				DrawGUI( );
			}

			TextureManager::CleanUpTextures( );

			window->SwapBuffers( );
		}

		TextureManager::RemoveEverything( );

		delete window;
	}

	void Application::Init( )
	{
		CatWare::Logging::InitLoggers( );

		CW_ENGINE_LOG->Info( "Initializing engine" );

		// temporary srand call - PT
		std::srand( time( NULL ) );

		CW_ENGINE_LOG->Info( "Initializing ImGui" );

		// initialze imgui - this is temporary
		ImGui::CreateContext( );

		ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

		ImGui::StyleColorsDark( );

		window = new Window( initConfig.windowTitle, initConfig.windowWidth, initConfig.windowHeight, initConfig.windowFullscreen );

		Renderer::SetScreenSize( window->GetWidth( ), window->GetHeight( ) );
		Renderer::Init( new Rendering::OpenGL::OpenGLAPI );

		Text::InitFreetype( );

		PostInit( );

		frameTimer.Reset( );
		tickTimer.Reset( );
	}

	void Application::Update( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->Update( );
		currentScene->entityManager.Update( );
		currentScene->physicsWorld.Update( );
	}

	void Application::Tick( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->Tick( );
		currentScene->entityManager.Tick( );
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
		Scene* currentScene = SceneManager::GetCurrentScene( );

		Vector2D oldRenderOffset = Renderer::renderOffset;
		Renderer::renderOffset = { 0, 0 };

		currentScene->DrawGUI( );

		Renderer::renderOffset = oldRenderOffset;

		// ImGui stuff
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplSDL2_NewFrame( );
		ImGui::NewFrame( );

		ImGui::ShowDemoWindow( nullptr );

		ImGui::Render( );

		ImGui::EndFrame( );

		ImGui::UpdatePlatformWindows( );
		ImGui::RenderPlatformWindowsDefault( );
	}
}