#include "OpenGLShader.h"

#include "CatWare/Debug/Debug.h"
#include "CatWare/Log.h"

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

				GLint i;
				GLint count;

				GLint size; // size of the variable
				GLenum type; // type of the variable (float, vec3 or mat4, etc)

				const GLsizei bufSize = 16; // maximum name length
				GLchar name[bufSize]; // variable name in GLSL
				GLsizei length; // name length

				glGetProgramiv( programID, GL_ACTIVE_UNIFORMS, &count );

				for ( i = 0; i < count; i++ )
				{
					glGetActiveUniform( programID, ( GLuint ) i, bufSize, &length, &size, &type, name );
					uniformIDs.insert( { std::string( name ), i } );
				}
			}

			OpenGLShader::~OpenGLShader( ) { glDeleteProgram( programID ); }

			void OpenGLShader::Bind( ) { glUseProgram( programID ); }

			void OpenGLShader::Unbind( ) { glUseProgram( 0 ); }

			void OpenGLShader::SetUniform1i( std::string name, int i )
			{
				glUniform1i( uniformIDs[name], i );
			}

			void OpenGLShader::SetUniform2i( std::string name, int i, int i2 )
			{
				glUniform2i( uniformIDs[name], i, i2 );
			}

			void OpenGLShader::SetUniform3i( std::string name, int i, int i2, int i3 )
			{
				glUniform3i( uniformIDs[name], i, i2, i3 );
			}

			void OpenGLShader::SetUniform4i( std::string name, int i, int i2, int i3, int i4 )
			{
				glUniform4i( uniformIDs[name], i, i2, i3, i4 );
			}

			void OpenGLShader::SetUniform1f( std::string name, float f )
			{
				glUniform1f( uniformIDs[name], f );
			}

			void OpenGLShader::SetUniform2f( std::string name, float f, float f2 )
			{
				glUniform2f( uniformIDs[name], f, f2 );
			}

			void OpenGLShader::SetUniform3f( std::string name, float f, float f2, float f3 )
			{
				glUniform3f( uniformIDs[name], f, f2, f3 );
			}

			void OpenGLShader::SetUniform4f( std::string name, float f, float f2, float f3, float f4 )
			{
				glUniform4f( uniformIDs[name], f, f2, f3, f4 );
			}

			void OpenGLShader::SetUniformMat4( std::string name, glm::mat4 mat )
			{
				glUniformMatrix4fv( uniformIDs[name], 1, true, &mat[0][0] );
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
		} // namespace OpenGL
	} // namespace Rendering
} // namespace CatWare
