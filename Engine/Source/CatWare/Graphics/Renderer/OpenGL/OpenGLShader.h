#pragma once

#include <glad/glad.h>

#include "CatWare/Graphics/Renderer/Shader.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			class OpenGLShader : public Shader
			{
			public:
				OpenGLShader( std::string vertexSource, std::string fragmentSource );
				virtual ~OpenGLShader( );

				virtual void Bind( ) override;
				virtual void Unbind( ) override;

				virtual void SetUniform1i( std::string name, int i ) override;
				virtual void SetUniform2i( std::string name, int i, int i2 ) override;
				virtual void SetUniform3i( std::string name, int i, int i2, int i3 ) override;
				virtual void SetUniform4i( std::string name, int i, int i2, int i3, int i4 ) override;

				virtual void SetUniform1f( std::string name, int f ) override;
				virtual void SetUniform2f( std::string name, int f, int f2 ) override;
				virtual void SetUniform3f( std::string name, int f, int f2, int f3 ) override;
				virtual void SetUniform4f( std::string name, int f, int f2, int f3, int f4 ) override;


			private:
				unsigned int programID;

				unsigned int CompileShader( std::string source, GLenum type );
			};
		}
	}
}