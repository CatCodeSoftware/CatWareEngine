#include "Application.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Utils/Color.h"
#include "Graphics/Renderer/Buffer.h"
#include "Graphics/Renderer/OpenGL/OpenGLBuffer.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/Texture.h"

namespace CatWare
{
	using namespace CatWare::Rendering;

	Application::Application( )
	{

	}

	Application::~Application( )
	{

	}

	void Application::Run( )
	{
		window = new Window( "CatWare", 1280, 720, true );

		Renderer::Init( new OpenGL::OpenGLAPI );
		Rendering::Renderer::SetScreenSize( window->GetWidth( ), window->GetHeight( ) );

		PostInit( );

		while ( running )
		{
			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			Update( );
			Draw( );

			window->SwapBuffers( );
		}

		delete window;
	}
}