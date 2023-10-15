#include "OpenGLShader.h"

#include "CatWare/Utils/Log.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			OpenGLShader::OpenGLShader( std::string vertexSource, std::string fragmentSource )
			{
				programID = glCreateProgram( );

				unsigned int vertexShader = CompileShader( vertexSource, GL_VERTEX_SHADER );
				unsigned int fragmentShader = CompileShader( fragmentSource, GL_FRAGMENT_SHADER );

				glAttachShader( programID, vertexShader );
				glAttachShader( programID, fragmentShader );

				glLinkProgram( programID );
			}

			OpenGLShader::~OpenGLShader( )
			{
				glDeleteProgram( programID );
			}

			void OpenGLShader::Bind( )
			{
				glUseProgram( programID );
			}

			void OpenGLShader::Unbind( )
			{
				glUseProgram( 0 );
			}

			unsigned int OpenGLShader::CompileShader( std::string source, GLenum type )
			{
				unsigned int shader = glCreateShader( type );

				int shaderLenght = source.length( );
				const char* shaderSourceC = source.c_str( );

				glShaderSource( shader, 1, &shaderSourceC, &shaderLenght );
				glCompileShader( shader );

				int isCompiled;
				glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );

				if ( !isCompiled )
				{
					int maxLength = 0;
					glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

					char* shaderLog = new char[maxLength];
					glGetShaderInfoLog( shader, maxLength, &maxLength, shaderLog );

					CW_ENGINE_LOG->Error( "Error compiling shader! %s", shaderLog );

					delete shaderLog;

					return 0;
				}

				return shader;
			}
		}
	}
}