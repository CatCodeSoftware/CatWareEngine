#include "Context.h"

#include <SDL.h>

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			OpenGLContext::OpenGLContext( SDL_Window* windowHandle )
			{
				this->windowHandle = windowHandle;
			}

			void OpenGLContext::Init( )
			{
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

				SDL_GL_CreateContext( windowHandle );
			}

			void OpenGLContext::SwapBuffers( )
			{
				SDL_GL_SwapWindow( windowHandle );
			}
		}
	}
}