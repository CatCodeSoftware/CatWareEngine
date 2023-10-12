#include <CatWare.h>

#include <iostream>

using namespace CatWare;

class Sandbox : public CatWare::Application
{
public:
	Sandbox( ) {}
	~Sandbox( ) {}

	void Run( )
	{
		CatWare::Logging::InitLoggers( );

		CW_LOG->Warning( "Running application" );

		Window* window = new Window( "Window", 800, 600, false );
		
		window->SetCursorShown( false );
		window->SetCursorLocked( true );

		while ( !window->ShouldClose( ) )
		{
			window->HandleWindowEvents( );
		}

	}
};

CW_REGISTER_APP( Sandbox )