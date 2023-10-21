#pragma once

#include "CatWare/Graphics/Renderer/Texture.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			class OpenGLTexture : public Texture2D
			{
			public:
				OpenGLTexture( int width, int height, void* data, unsigned int channels );
				virtual ~OpenGLTexture( );
				
				virtual void Bind( unsigned int slot ) override;
				virtual void Unbind( ) override;

				virtual void SetTextureFilter( TextureFilter tf ) override;

				inline virtual UInt32 GetTextureID( ) override { return textureID; }

			private:
				int boundSlot = -1;
				unsigned int textureID;
			};
		}
	}
}