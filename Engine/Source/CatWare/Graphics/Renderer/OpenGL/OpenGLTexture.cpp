#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			OpenGLTexture::OpenGLTexture( int width, int height, void* rgbaData )
			{
				glGenTextures( 1, &textureID );
				glBindTexture( GL_TEXTURE_2D, textureID );

				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );// set billinear by default
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData );
				glBindTexture( GL_TEXTURE_2D, 0 );
			}

			OpenGLTexture::~OpenGLTexture( )
			{
				glDeleteTextures( 1, &textureID );
			}

			void OpenGLTexture::Bind( unsigned int slot )
			{
				glActiveTexture( GL_TEXTURE0 + slot );
				glBindTexture( GL_TEXTURE_2D, textureID );

				boundSlot = slot;
			}

			void OpenGLTexture::SetTextureFilter( TextureFilter tf )
			{

			}

			void OpenGLTexture::Unbind( )
			{
				if ( boundSlot >= 0 )
				{
					glActiveTexture( GL_TEXTURE0 + boundSlot );
				}

				glBindTexture( GL_TEXTURE_2D, 0 );
			}
		}
	}
}
