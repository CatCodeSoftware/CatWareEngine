#include "Shader.h"

#include <fstream>

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

		Shader* Shader::CreateFromFile( std::string vertexPath, std::string fragmentPath )
		{
			// read the files
			std::ifstream vertexFile( vertexPath );
			std::ifstream fragmentFile( fragmentPath );

			std::string vertexSource;
			std::string fragmentSource;

			std::string tempString;

			while ( std::getline( vertexFile, tempString ) )
			{
				vertexSource += tempString + '\n';
			}

			while ( std::getline( fragmentFile, tempString ) )
			{
				fragmentSource += tempString + '\n';
			}

			vertexFile.close( );
			fragmentFile.close( );

			return Shader::Create( vertexSource, fragmentSource );
		}
	}
}