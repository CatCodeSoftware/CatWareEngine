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

			void OpenGLShader::SetUniform1i( std::string name, int i )
			{
				glUniform1i( glGetUniformLocation( programID, name.c_str( ) ), i );
			}

			void OpenGLShader::SetUniform2i( std::string name, int i, int i2 )
			{
				glUniform2i( glGetUniformLocation( programID, name.c_str( ) ), i, i2 );
			}

			void OpenGLShader::SetUniform3i( std::string name, int i, int i2, int i3 )
			{
				glUniform3i( glGetUniformLocation( programID, name.c_str( ) ), i, i2, i3 );
			}

			void OpenGLShader::SetUniform4i( std::string name, int i, int i2, int i3, int i4 )
			{
				glUniform4i( glGetUniformLocation( programID, name.c_str( ) ), i, i2, i3, i4 );
			}

			void OpenGLShader::SetUniform1f( std::string name, int f )
			{
				glUniform1f( glGetUniformLocation( programID, name.c_str( ) ), f );
			}

			void OpenGLShader::SetUniform2f( std::string name, int f, int f2 )
			{
				glUniform2f( glGetUniformLocation( programID, name.c_str( ) ), f, f2 );
			}

			void OpenGLShader::SetUniform3f( std::string name, int f, int f2, int f3 )
			{
				glUniform3f( glGetUniformLocation( programID, name.c_str( ) ), f, f2, f3 );
			}

			void OpenGLShader::SetUniform4f( std::string name, int f, int f2, int f3, int f4 )
			{
				glUniform4f( glGetUniformLocation( programID, name.c_str( ) ), f, f2, f3, f4 );
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

					delete[] shaderLog;

					return 0;
				}

				return shader;
			}
		}
	}
}