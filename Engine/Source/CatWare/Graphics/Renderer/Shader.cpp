#include "Shader.h"

#include "RendererAPI.h"
#include "Renderer.h"

#include "OpenGL/OpenGLShader.h"

namespace CatWare
{
	namespace Rendering
	{
		Shader* Shader::Create( std::string vertexSource, std::string fragmentSource )
		{
			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::OPENGL: return new OpenGL::OpenGLShader( vertexSource, fragmentSource );
			}
		}
	}
}