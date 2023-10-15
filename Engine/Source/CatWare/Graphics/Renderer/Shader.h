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
			
			static Shader* Create( std::string vertexSource, std::string fragmentSource );
		};
	}
}