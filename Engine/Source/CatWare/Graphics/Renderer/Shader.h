#pragma once

#include <string>
#include <glm/glm.hpp>

namespace CatWare
{
	namespace Rendering
	{
		enum class ShaderType
		{
			VERTEX, FRAGMENT
		};

		class Shader
		{
		public:
			virtual ~Shader( ) {}

			virtual void Bind( ) = 0;
			virtual void Unbind( ) = 0;


			virtual void SetUniform1i( std::string name, int i ) = 0;
			virtual void SetUniform2i( std::string name, int i, int i2 ) = 0;
			virtual void SetUniform3i( std::string name, int i, int i2, int i3 ) = 0;
			virtual void SetUniform4i( std::string name, int i, int i2, int i3, int i4 ) = 0;
			
			virtual void SetUniform1f( std::string name, float f ) = 0;
			virtual void SetUniform2f( std::string name, float f, float f2 ) = 0;
			virtual void SetUniform3f( std::string name, float f, float f2, float f3 ) = 0;
			virtual void SetUniform4f( std::string name, float f, float f2, float f3, float f4 ) = 0;

			virtual void SetUniformMat4( std::string name, glm::mat4 mat ) = 0;

			static Shader* Create( std::string vertexSource, std::string fragmentSource );
			static Shader* CreateFromFile( std::string vertexPath, std::string fragmentPath );
		};
	}
}