#include "Texture.h"

#include <stb_image.h>

#include "CatWare/Debug/Debug.h"

#include "RendererAPI.h"
#include "Renderer.h"
#include "OpenGL/OpenGLTexture.h"
#include "CatWare/Utils/Log.h"

namespace CatWare
{
	namespace Rendering
	{
		Texture2D* Texture2D::Create( int width, int height, void* data, unsigned int channels, TextureFilter textureFilter )
		{
			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::OPENGL: return new OpenGL::OpenGLTexture( width, height, data, channels, textureFilter );
			}

			return nullptr;
		}

		Texture2D* Texture2D::Create( std::string path, TextureFilter tf )
		{
			int width, height;

			void* rgbaData = stbi_load( path.c_str( ), &width, &height, nullptr, 4 );

			if ( rgbaData == NULL )
			{
				CW_ENGINE_LOG->Error( "Failed to load texture: %s", stbi_failure_reason( ) );

				return nullptr;
			}

			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::OPENGL: return new OpenGL::OpenGLTexture( width, height, rgbaData, 4, tf );
			}

			return nullptr;
		}
	}
}