#pragma once

#include <string>

#include "CatWare/Core.h"
#include "Renderer/Context.h"

struct SDL_Window;

namespace CatWare
{
	class CATWARE_API Window
	{
	public:
		Window( const std::string& title, unsigned int width, unsigned int height, bool isFullscreen, const bool resizable );
		~Window( );

		void HandleWindowEvents( );

		void SetSize( unsigned int width, unsigned int height );
		void SetTitle( std::string title );
		void SetFullscreen( bool isFullscreen );
		void SetCursorShown( bool isCursorShown );
		void SetCursorLocked( bool cursorLocked );

		unsigned int GetWidth( );
		unsigned int GetHeight( );
		std::string GetTitle( );
		bool GetFullscreen( );
		bool GetCursorShown( );
		bool GetCursorLocked( );

		bool ShouldClose( );

		void SwapBuffers( );

	private:
		SDL_Window* sdlWindow = nullptr;
		Rendering::RenderingContext* renderingContext = nullptr;
		
		unsigned int width, height;
		std::string title;

		bool isFullscreen = false;
		bool isCursorShown = true;
		bool isCursorLocked = false;

		bool shouldClose = false;
	};
}