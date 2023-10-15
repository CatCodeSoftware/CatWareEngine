#include "OpenGLContext.h"

#include <glad/glad.h>

#include "CatWare/Utils/Log.h"

void GLAPIENTRY
MessageCallback( GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam )
{
	fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
		type, severity, message );
}

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
				CW_ENGINE_LOG->Info( "Initializing OpenGL" );

				SDL_GL_LoadLibrary( NULL );

				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

				glContext = SDL_GL_CreateContext( windowHandle );

				// Load glad
				gladLoadGLLoader( SDL_GL_GetProcAddress );

				if ( !glContext )
				{
					CW_ENGINE_LOG->Error( "Failed to create OpenGL context: %s", SDL_GetError( ) );
				}

				CW_ENGINE_LOG->Info( "Vendor:   %s", glGetString( GL_VENDOR ) );
				CW_ENGINE_LOG->Info( "Renderer: %s", glGetString( GL_RENDERER ) );
				CW_ENGINE_LOG->Info( "Version:  %s", glGetString( GL_VERSION ) );

				glEnable( GL_PROGRAM_OUTPUT );
				glDebugMessageCallback( MessageCallback, 0 );
			}

			void OpenGLContext::SwapBuffers( )
			{
				SDL_GL_SwapWindow( windowHandle );
			}
		}
	}
}