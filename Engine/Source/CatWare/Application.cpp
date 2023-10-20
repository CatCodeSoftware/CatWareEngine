#include "Application.h"

#include <stb_image.h>
#include <random>

#include "Utils/Color.h"
#include "Graphics/Renderer/Buffer.h"
#include "Graphics/Renderer/OpenGL/OpenGLBuffer.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/Texture.h"

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

		window = new Window( "CatWare", 1280, 720, false );

		Renderer::Init( new OpenGL::OpenGLAPI );
		Rendering::Renderer::SetScreenSize( window->GetWidth( ), window->GetHeight( ) );

		PostInit( );

		while ( running )
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now( );

			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			Update( );
			Draw( );

			window->SwapBuffers( );

			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now( ) - startTime );
			deltaTime = double ( milliseconds.count( ) ) / 1000;
		}

		delete window;
	}
}