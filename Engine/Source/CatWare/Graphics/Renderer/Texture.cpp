#include "Texture.h"

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
	}
}