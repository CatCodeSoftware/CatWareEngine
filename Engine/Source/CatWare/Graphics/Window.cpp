#include "Window.h"

namespace CatWare
{
	Window::Window( std::string title, unsigned int width, unsigned int height, bool isFullscreen )
	{
		sdlWindow = SDL_CreateWindow( title.c_str( ), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN );
		SetFullscreen( isFullscreen );

		this->title = title;
		this->width = width;
		this->height = height;
		this->isFullscreen = isFullscreen;
	}


	void Window::HandleWindowEvents( )
	{
		SDL_Event e;

		while ( SDL_PollEvent( &e ) )
		{
			switch ( e.type )
			{
			case SDL_QUIT:
				shouldClose = true;
				break;
			}
		}
	}


	void Window::SetSize( unsigned int width, unsigned int height )
	{
		SDL_SetWindowSize( sdlWindow, width, height );

		this->width = width;
		this->height = height;
	}

	void Window::SetTitle( std::string title )
	{
		SDL_SetWindowTitle( sdlWindow, title.c_str( ) );
	}

	void Window::SetFullscreen( bool isFullscreen )
	{
		Uint32 flags;

		if ( isFullscreen )
			flags = SDL_WINDOW_FULLSCREEN;
		else
			flags = NULL;

		SDL_SetWindowFullscreen( sdlWindow, flags );

		this->isFullscreen = isFullscreen;
	}

	void Window::SetCursorShown( bool cursorShown )
	{
		SDL_ShowCursor( cursorShown );

		isCursorShown = cursorShown;
	}

	void Window::SetCursorLocked( bool cursorLocked )
	{
		SDL_SetRelativeMouseMode( ( SDL_bool ) cursorLocked );
	}

	unsigned int Window::GetWidth( ) { return width; }

	unsigned int Window::GetHeight( ) { return height; }

	std::string Window::GetTitle( ) { return title; }

	bool Window::GetFullscreen( ) { return isFullscreen; }

	bool Window::GetCursorShown( ) { return isCursorShown; }

	bool Window::GetCursorLocked( ) { return isCursorLocked; }

	
	bool Window::ShouldClose( ) { return shouldClose; }
}