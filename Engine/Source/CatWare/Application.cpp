#include "Application.h"

#include <random>
#include <sstream>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "CatWare/Filesystem/Filesystem.h"
#include "CatWare/Filesystem/Sources.h"
#include "Debug/Debug.h"
#include "Log.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Text.h"
#include "Assets/Assets.h"
#include "Input/Binds.h"
#include "Random.h"
#include "CatWare/Debug/DebugUI.h"

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

			const Scene *currentScene = SceneManager::GetCurrentScene( );

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


			while ( !frameTimer.HasTimeElapsed( 1 / Time::maxFPS ) && Time::frameRateLimited )
			{
			}
			Time::SetDeltaTime( frameTimer.GetTime( ) );

			window->SwapBuffers( );
		}

		DeInit( );
	}

	void Application::Init( )
	{
		Logging::InitLoggers( );
		Console::Init( );

		CW_ENGINE_LOG->Info( "Initializing engine" );

		/*
		#ifdef CW_DEBUG
			CW_ENGINE_LOG->Warning( "Memory debugging enabled" );
		#endif
		*/

		CW_ENGINE_LOG->Info( "Initializing filesystem" );
		FileSystem::AddSource( new DirectorySource( "Custom" ) );
		FileSystem::AddSource( new DirectorySource( "." ) );

		Random::Init( );

		// initialze imgui - this is temporary
		ImGui::CreateContext( );

		ImGuiIO &io = ImGui::GetIO( );
		( void ) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

		io.Fonts->AddFontFromFileTTF( "EngineRes/Fonts/IBMPlexMono-Medium.ttf", 18 );

		// imgui style i stole from the internet
		ImVec4 *colors = ImGui::GetStyle( ).Colors;
		colors[ImGuiCol_Text] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
		colors[ImGuiCol_TextDisabled] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
		colors[ImGuiCol_WindowBg] = ImVec4( 0.15f, 0.15f, 0.15f, 1.00f );
		colors[ImGuiCol_ChildBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
		colors[ImGuiCol_PopupBg] = ImVec4( 0.19f, 0.19f, 0.19f, 0.92f );
		colors[ImGuiCol_Border] = ImVec4( 0.39f, 0.39f, 0.39f, 1.0f );
		colors[ImGuiCol_BorderShadow] = ImVec4( 0.00f, 0.00f, 0.00f, 0.24f );
		colors[ImGuiCol_FrameBg] = ImVec4( 0.05f, 0.05f, 0.05f, 0.54f );
		colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.19f, 0.19f, 0.19f, 0.54f );
		colors[ImGuiCol_FrameBgActive] = ImVec4( 0.20f, 0.22f, 0.23f, 1.00f );
		colors[ImGuiCol_TitleBg] = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_TitleBgActive] = ImVec4( 0.06f, 0.06f, 0.06f, 1.00f );
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_MenuBarBg] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.05f, 0.05f, 0.05f, 0.54f );
		colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.34f, 0.34f, 0.34f, 0.54f );
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.40f, 0.40f, 0.40f, 0.54f );
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.56f, 0.56f, 0.56f, 0.54f );
		colors[ImGuiCol_CheckMark] = ImVec4( 0.33f, 0.67f, 0.86f, 1.00f );
		colors[ImGuiCol_SliderGrab] = ImVec4( 0.34f, 0.34f, 0.34f, 0.54f );
		colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.56f, 0.56f, 0.56f, 0.54f );
		colors[ImGuiCol_Button] = ImVec4( 0.05f, 0.05f, 0.05f, 0.54f );
		colors[ImGuiCol_ButtonHovered] = ImVec4( 0.19f, 0.19f, 0.19f, 0.54f );
		colors[ImGuiCol_ButtonActive] = ImVec4( 0.20f, 0.22f, 0.23f, 1.00f );
		colors[ImGuiCol_Header] = ImVec4( 0.00f, 0.00f, 0.00f, 0.52f );
		colors[ImGuiCol_HeaderHovered] = ImVec4( 0.00f, 0.00f, 0.00f, 0.36f );
		colors[ImGuiCol_HeaderActive] = ImVec4( 0.20f, 0.22f, 0.23f, 0.33f );
		colors[ImGuiCol_Separator] = ImVec4( 0.28f, 0.28f, 0.28f, 0.29f );
		colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.44f, 0.44f, 0.44f, 0.29f );
		colors[ImGuiCol_SeparatorActive] = ImVec4( 0.40f, 0.44f, 0.47f, 1.00f );
		colors[ImGuiCol_ResizeGrip] = ImVec4( 0.28f, 0.28f, 0.28f, 0.29f );
		colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.44f, 0.44f, 0.44f, 0.29f );
		colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.40f, 0.44f, 0.47f, 1.00f );
		colors[ImGuiCol_Tab] = ImVec4( 0.00f, 0.00f, 0.00f, 0.52f );
		colors[ImGuiCol_TabHovered] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ImGuiCol_TabActive] = ImVec4( 0.20f, 0.20f, 0.20f, 0.36f );
		colors[ImGuiCol_TabUnfocused] = ImVec4( 0.00f, 0.00f, 0.00f, 0.52f );
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ImGuiCol_DockingPreview] = ImVec4( 0.33f, 0.67f, 0.86f, 1.00f );
		colors[ImGuiCol_DockingEmptyBg] = ImVec4( 1.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_PlotLines] = ImVec4( 1.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_PlotLinesHovered] = ImVec4( 1.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_PlotHistogram] = ImVec4( 1.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 1.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_TableHeaderBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.52f );
		colors[ImGuiCol_TableBorderStrong] = ImVec4( 0.39f, 0.39f, 0.39f, 1.0f );
		colors[ImGuiCol_TableBorderLight] = ImVec4( 0.39f, 0.39f, 0.39f, 1.0f );
		colors[ImGuiCol_TableRowBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
		colors[ImGuiCol_TableRowBgAlt] = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
		colors[ImGuiCol_TextSelectedBg] = ImVec4( 0.20f, 0.22f, 0.23f, 1.00f );
		colors[ImGuiCol_DragDropTarget] = ImVec4( 0.33f, 0.67f, 0.86f, 1.00f );
		colors[ImGuiCol_NavHighlight] = ImVec4( 1.00f, 0.00f, 0.00f, 1.00f );
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 1.00f, 0.00f, 0.00f, 0.70f );
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4( 1.00f, 0.00f, 0.00f, 0.20f );
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4( 1.00f, 0.00f, 0.00f, 0.35f );

		window = new Window(
			initConfig.windowTitle, initConfig.windowWidth, initConfig.windowHeight, initConfig.windowFullscreen,
			initConfig.windowResizable
			);

		CW_ENGINE_LOG->Info( "Initializing renderer" );
		renderingAPI = new Rendering::OpenGL::OpenGLAPI;
		Renderer::Init( renderingAPI, initConfig.windowWidth, initConfig.windowHeight );

		// AudioEngine::InitAudio( );

		Text::InitFreetype( );

		PostInit( );

		Console::RegisterConVar( "time_FPSLimited", ConVar( ConVarType::BOOL, &Time::frameRateLimited ) );
		Console::RegisterConVar( "time_maxFPS", ConVar( ConVarType::FLOAT, &Time::maxFPS ) );
		Console::RegisterConVar( "time_modifier", ConVar( ConVarType::FLOAT, &Time::modifier ) );
		Console::RegisterConVar( "time_tickPerSecond", ConVar( ConVarType::FLOAT, &Time::ticksPerSecond ) );

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

		/*
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
		*/
	}

	void Application::Update( )
	{
		Scene *currentScene = SceneManager::GetCurrentScene( );

		currentScene->Update( );
		currentScene->world.entities.Update( );

		// AudioEngine::UpdateHandles( );
	}

	void Application::Tick( )
	{
		Scene *currentScene = SceneManager::GetCurrentScene( );

		currentScene->Tick( );
		currentScene->world.physicsWorld.Step( 1.0f / Time::ticksPerSecond );
		currentScene->world.entities.Tick( );
	}

	void Application::Draw( )
	{
		Scene *currentScene = SceneManager::GetCurrentScene( );

		Renderer::StartDrawing( );

		currentScene->Draw( );
		currentScene->world.entities.Draw( );

		Renderer::EndDrawing( );
	}

	void Application::DrawGUI( )
	{
		OrthoCamera *uiCamera = new OrthoCamera( window->GetWidth( ), window->GetHeight( ) );
		OrthoCamera *oldCamera = Renderer::camera2D;

		Renderer::camera2D = uiCamera;

		Scene *currentScene = SceneManager::GetCurrentScene( );

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplSDL2_NewFrame( );
		ImGui::NewFrame( );

		currentScene->DrawGUI( );

		DebugUI::Draw( );

		ImGui::Render( );

		ImGui::EndFrame( );

		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

		ImGui::UpdatePlatformWindows( );
		ImGui::RenderPlatformWindowsDefault( );

		Renderer::camera2D = oldCamera;
		delete uiCamera;
	}
}
