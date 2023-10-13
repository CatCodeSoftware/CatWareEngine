#pragma once

#include "CatWare/Graphics/Renderer/Context.h"

class SDL_Window;

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			class OpenGLContext : public RenderingContext
			{
			public:
				OpenGLContext( SDL_Window* windowHandle );

				// Inherited via RenderingContext
				virtual void Init( ) override;
				virtual void SwapBuffers( ) override;

			private:
				SDL_Window* windowHandle;
			};
		}
	}
}