#pragma once

#include <SDL.h>

#include "CatWare/Core.h"
#include "CatWare/Graphics/Renderer/Context.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			class CATWARE_API OpenGLContext : public RenderingContext
			{
			public:
				OpenGLContext( SDL_Window* windowHandle );

				// Inherited via RenderingContext
				virtual void Init( ) override;
				virtual void SwapBuffers( ) override;

			private:
				SDL_Window* windowHandle;
				SDL_GLContext glContext;
			};
		}
	}
}