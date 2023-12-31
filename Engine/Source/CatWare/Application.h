/**
*	@file Application.h
*
*	@brief File for the CatWare::Application class.
*
*	@author PointThink
*/

#pragma once

#include <chrono>

#include "Core.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer/Renderer.h"
#include "GameState/Scene.h"
#include "Time.h"

namespace CatWare
{
	// We need to make these weird comments because doxygen is fucking stupid - PT

	/**
	* Contains information used by CatWare::Application when initializing.
	* Needs to be modified before in the chid class constructor.
	*/
	class InitConfig
	{
	public:
		std::string windowTitle = "CatWare";
		UInt16 windowWidth = 1280;
		UInt16 windowHeight = 720;
		bool windowFullscreen = false;
		bool windowResizable = false;

		Rendering::RendererAPI::API renderAPI = Rendering::RendererAPI::API::OPENGL;
	};

	/**
	* Singleton class for the entire application.
	* Handles initialization, the game loop and deinitialization
	*/
	class CATWARE_API Application
	{
	public:
		void Run( );

		// Overridable methods
		virtual void PostInit( ) = 0;

		virtual void PreDeInit( )
		{
		}

	protected:
		bool running = true;

		Window* window = nullptr;
		InitConfig initConfig;

	private:
		Rendering::OpenGL::OpenGLAPI* renderingAPI;

		InternalTimer frameTimer;
		InternalTimer tickTimer;

		void Init( );
		void DeInit( );
		void Update( );
		void Tick( );
		void Draw( );
		void DrawGUI( );
	};
}
