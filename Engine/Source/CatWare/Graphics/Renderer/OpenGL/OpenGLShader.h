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

			private:
				unsigned int programID;

				unsigned int CompileShader( std::string source, GLenum type );
			};
		}
	}
}