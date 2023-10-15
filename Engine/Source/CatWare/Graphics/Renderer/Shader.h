#pragma once

#include <string>

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
			
			virtual void SetUniform1f( std::string name, int f ) = 0;
			virtual void SetUniform2f( std::string name, int f, int f2 ) = 0;
			virtual void SetUniform3f( std::string name, int f, int f2, int f3 ) = 0;
			virtual void SetUniform4f( std::string name, int f, int f2, int f3, int f4 ) = 0;


			static Shader* Create( std::string vertexSource, std::string fragmentSource );
		};
	}
}