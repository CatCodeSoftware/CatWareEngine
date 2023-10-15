#pragma once

#include <string>

#include "CatWare/Core.h"

namespace CatWare
{
	namespace Rendering
	{
		enum class TextureFilter
		{
			NEAREST, BILINEAR
		};

		class CATWARE_API Texture2D
		{
		public:
			virtual ~Texture2D( ) {}
			
			virtual void SetTextureFilter( TextureFilter tf ) = 0;

			virtual void Bind( unsigned int slot ) = 0;
			virtual void Unbind( ) = 0;

			static Texture2D* Create( int width, int height, void* rgbaData );
			static Texture2D* Create( std::string path );
		};
	}
}