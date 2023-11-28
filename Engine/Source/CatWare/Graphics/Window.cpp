#include "Window.h"

#include <backends/imgui_impl_sdl2.h>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Input/KeyboardAndMouse.h"
#include "CatWare/Log.h"

#include "Renderer/OpenGL/OpenGLContext.h"
#include "Renderer/Renderer.h"

#include "..\Error.h"

namespace CatWare
{
	Window::Window( std::string title, unsigned int width, unsigned int height, bool isFullscreen )
	{
		CW_ENGINE_LOG->Info( "Creating %dx%d window with title %s", width, height, title.c_str( ) );

		sdlWindow = SDL_CreateWindow( title.c_str( ), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

		SetFullscreen( isFullscreen );

		if ( sdlWindow == NULL )
			CW_ABORT( "Failed to create SDL Window: " + std::string( SDL_GetError( ) ) );

		this->title = title;
		this->width = width;
		this->height = height;
		this->isFullscreen = isFullscreen;

		// Create rendering context
		renderingContext = new Rendering::OpenGL::OpenGLContext( sdlWindow ); // only OpenGL for now

		renderingContext->Init( );
	}

	Window::~Window( )
	{
		CW_ENGINE_LOG->Info( "Closing window" );
		SDL_DestroyWindow( sdlWindow );

		delete renderingContext;
	}

	void Window::HandleWindowEvents( )
	{
		SDL_Event e;

		while ( SDL_PollEvent( &e ) )
		{
			ImGui_ImplSDL2_ProcessEvent( &e );

			switch ( e.type )
			{
			case SDL_QUIT:
				shouldClose = true;
				break;


			case SDL_KEYDOWN:
				Input::SetKeyPressed( e.key.keysym.scancode, true );
				break;

			case SDL_KEYUP:
				Input::SetKeyPressed( e.key.keysym.scancode, false );
				break;

			case SDL_MOUSEBUTTONDOWN:
				Input::SetMousePressed( e.button.button, true );
				break;

			case SDL_MOUSEBUTTONUP:
				Input::SetMousePressed( e.button.button, false );
				break;


			case SDL_MOUSEMOTION:
				Input::SetMouseMotion( { ( double ) e.motion.x, ( double ) e.motion.y } );
			}
		}
	}

	void Window::SwapBuffers( )
	{
		renderingContext->SwapBuffers( );
	}


	void Window::SetSize( unsigned int width, unsigned int height )
	{
		SDL_SetWindowSize( sdlWindow, width, height );

		this->width = width;
		this->height = height;

		Renderer::SetScreenSize( width, height );
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