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

	protected:
		bool running = true;

		Window* window = nullptr;
		InitConfig initConfig;

	private:
		InternalTimer frameTimer;
		InternalTimer tickTimer;

		float ticksPerSecond = 60; // 60 seems like a nice default value

		void Init( );
		void Update( );
		void Tick( );
		void Draw( );
		void DrawGUI( );
	};
}