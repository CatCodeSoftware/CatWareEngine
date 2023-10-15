#include "Texture.h"

#include <stb_image.h>

#include "RendererAPI.h"
#include "Renderer.h"
#include "OpenGL/OpenGLTexture.h"

namespace CatWare
{
	namespace Rendering
	{
		Texture2D* Texture2D::Create( int width, int height, void* rgbaData )
		{
			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::OPENGL: return new OpenGL::OpenGLTexture( width, height, rgbaData );
			}
		}

		Texture2D* Texture2D::Create( std::string path )
		{
			int width, height;

			void* rgbaData = stbi_load( path.c_str( ), &width, &height, nullptr, 4 );

			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::OPENGL: return new OpenGL::OpenGLTexture( width, height, rgbaData );
			}
		}
	}
}