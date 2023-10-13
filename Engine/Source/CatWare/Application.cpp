#include "Application.h"

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
		window = new Window( "CatWare", 1280, 720, false );

		PostInit( );

		while ( running )
		{
			window->HandleWindowEvents( );

			running = !window->ShouldClose( );

			window->SwapBuffers( );
		}
	}
}