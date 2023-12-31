#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "CatWare/Debug/Debug.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			void OpenGLAPI::SetClearColor( Color clearColor )
			{
				glClearColor( float( clearColor.r ) / 255.0f, float( clearColor.g ) / 255.0f, float( clearColor.b ) / 255.0f, float( clearColor.a ) / 255.0f );
			}

			void OpenGLAPI::Clear( )
			{
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			}

			void OpenGLAPI::SetViewportSize( int width, int height )
			{
				glViewport( 0, 0, width, height );
			}

			void OpenGLAPI::SetBlendAlpha( bool blend )
			{
				if ( blend )
				{
					glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
					glEnable( GL_BLEND );
				}
				else
				{
					glDisable( GL_BLEND );
				}
			}

			void OpenGLAPI::DrawIndexed( VertexArray* vertexArray )
			{
				vertexArray->Bind( );
				glDrawElements( GL_TRIANGLES, vertexArray->GetIndexBuffer( )->GetCount( ), GL_UNSIGNED_INT, nullptr );
			}

			RendererAPI::API OpenGLAPI::GetRenderAPI( )
			{
				return RendererAPI::API::OPENGL;
			}
		}
	}
}