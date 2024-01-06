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

			Scene* currentScene = SceneManager::GetCurrentScene( );

			if ( currentScene != nullptr )
			{
				Update( );

				int ticksElapsed = tickTimer.TimesTimeElapsed( ( 1.0 / Time::ticksPerSecond ) / Time::modifier );

				if ( ticksElapsed != 0 )
				{
					tickTimer.Reset( );
				}

				for ( ; ticksElapsed > 0; ticksElapsed-- )
				{
					Tick( );
				}

				currentScene->world.entities.DestroyEntities( );
				currentScene->world.physicsWorld.RemoveItems( );

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

		ImGuiIO& io = ImGui::GetIO( );
		( void ) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

		io.Fonts->AddFontFromFileTTF( "EngineRes/Fonts/IBMPlexMono-Medium.ttf", 16 );

		// Setup style
		// Moonlight style by Madam-Herta from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();
		
		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.WindowRounding = 11.5f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
		style.FrameRounding = 11.89999961853027f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
		style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
		style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
		style.IndentSpacing = 0.0f;
		style.ColumnsMinSpacing = 4.900000095367432f;
		style.ScrollbarSize = 11.60000038146973f;
		style.ScrollbarRounding = 15.89999961853027f;
		style.GrabMinSize = 3.700000047683716f;
		style.GrabRounding = 20.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
		
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411764889955521f, 0.1019607856869698f, 0.1176470592617989f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1137254908680916f, 0.125490203499794f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7960784435272217f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1803921610116959f, 0.1882352977991104f, 0.196078434586525f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.1411764770746231f, 0.1647058874368668f, 0.2078431397676468f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.1294117718935013f, 0.1490196138620377f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125490203499794f, 0.2745098173618317f, 0.572549045085907f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8823529481887817f, 0.7960784435272217f, 0.5607843399047852f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95686274766922f, 0.95686274766922f, 0.95686274766922f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2666666805744171f, 0.2901960909366608f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);

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
		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->Update( );
		currentScene->world.entities.Update( );

		// AudioEngine::UpdateHandles( );
	}

	void Application::Tick( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		currentScene->Tick( );
		currentScene->world.physicsWorld.Step( 1.0f / Time::ticksPerSecond );
		currentScene->world.entities.Tick( );
	}

	void Application::Draw( )
	{
		Scene* currentScene = SceneManager::GetCurrentScene( );

		Renderer::StartDrawing( );

		currentScene->Draw( );
		currentScene->world.entities.Draw( );
		currentScene->world.particles.Draw( );

		if ( currentScene->world.physicsWorld.drawColliders )
			currentScene->world.physicsWorld.DrawColliders( );

		Renderer::EndDrawing( );
	}

	void Application::DrawGUI( )
	{
		OrthoCamera* uiCamera = new OrthoCamera( window->GetWidth( ), window->GetHeight( ) );
		OrthoCamera* oldCamera = Renderer::camera2D;

		Renderer::camera2D = uiCamera;

		Scene* currentScene = SceneManager::GetCurrentScene( );

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplSDL2_NewFrame( );
		ImGui::NewFrame( );

		currentScene->DrawGUI( );

		DebugUI::Draw( );

		ImGui::Render( );

		ImGui::EndFrame( );

		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

		// ImGui::UpdatePlatformWindows( );
		// ImGui::RenderPlatformWindowsDefault( );

		Renderer::camera2D = oldCamera;
		delete uiCamera;
	}
}
