#include "Context.h"

#include <glad/glad.h>

#include "CatWare/Utils/Log.h"

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
				SDL_GL_LoadLibrary( NULL );

				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

				glContext = SDL_GL_CreateContext( windowHandle );

				// Load glad
				int version = gladLoadGLLoader( SDL_GL_GetProcAddress );

				CW_ENGINE_LOG->Info( "OpenGL version %d", version );

				if ( !glContext )
				{
					CW_ENGINE_LOG->Error( "Failed to create OpenGL context: %s", SDL_GetError( ) );
				}
			}

			void OpenGLContext::SwapBuffers( )
			{
				SDL_GL_SwapWindow( windowHandle );
			}
		}
	}
}