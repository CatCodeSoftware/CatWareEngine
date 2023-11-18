#include "Shader.h"

#include <fstream>

#include "CatWare/Debug/Debug.h"

#include "RendererAPI.h"
#include "Renderer.h"
#include "OpenGL/OpenGLShader.h"
#include "CatWare/Utils/Log.h"

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

			return nullptr;
		}

		Shader* Shader::CreateFromFile( std::string vertexPath, std::string fragmentPath )
		{
			CW_ENGINE_LOG->Info( "Loading shader from files:\n\tVertex: %s\n\tFragment: %s", vertexPath.c_str( ), fragmentPath.c_str( ) );

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