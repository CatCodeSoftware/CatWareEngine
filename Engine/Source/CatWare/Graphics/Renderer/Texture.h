#pragma once

#include <string>

#include "CatWare/Types/Types.h"
#include "CatWare/Core.h"

namespace CatWare
{
	namespace Rendering
	{
		enum class TextureFilter
		{
			NEAREST, LINEAR
		};

		class CATWARE_API Texture2D
		{
		public:
			virtual ~Texture2D( ) {}
			
			virtual void SetTextureFilter( TextureFilter tf ) = 0;

			virtual void Bind( unsigned int slot ) = 0;
			virtual void Unbind( ) = 0;

			virtual UInt32 GetTextureID( ) = 0;

			virtual UInt32 GetTextureWidth( ) = 0;
			virtual UInt32 GetTextureHeight( ) = 0;

			virtual void SetWrapped( bool wrapped ) = 0;

			static Texture2D* Create( int width, int height, void* buffer, unsigned int channels = 4, TextureFilter textureFilter = TextureFilter::LINEAR );
			static Texture2D* Create( std::string path, TextureFilter tf );
		};
	}
}