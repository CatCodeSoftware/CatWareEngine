#pragma once

#include <chrono>

#include "Core.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer/Renderer.h"
#include "Scene.h"
#include "Utils/Time.h"

namespace CatWare
{
	class InitConfig
	{
	public:
		std::string windowTitle = "CatWare";
		UInt16 windowWidth = 1280;
		UInt16 windowHeight = 720;
		bool windowFullscreen = false;

		Rendering::RendererAPI::API renderAPI = Rendering::RendererAPI::API::OPENGL;
	};

	class CATWARE_API Application
	{
	public:
		Application( );
		virtual ~Application( );

		virtual void Run( ) final;

		// Overridable methods
		virtual void PostInit( ) {}
		virtual void PreDeInit( ) {}

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