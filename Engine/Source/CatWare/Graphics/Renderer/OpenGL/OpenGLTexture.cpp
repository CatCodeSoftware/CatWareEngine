#include "OpenGLTexture.h"

#include "CatWare/Debug/Debug.h"

#include <glad/glad.h>

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			OpenGLTexture::OpenGLTexture( int width, int height, void* data, unsigned int channels, TextureFilter tf )
			{
				GLenum internalFormat = GL_RGBA8;
				GLenum format = GL_RGBA;

				switch ( channels )
				{
				case 1:
					internalFormat = GL_RED;
					format = GL_RED;
					break;

				case 2:
					internalFormat = GL_RG8;
					format = GL_RG;
					break;

				case 3:
					internalFormat = GL_RGB8;
					format = GL_RGB;
					break;

				case 4:
					internalFormat = GL_RGBA8;
					format = GL_RGBA;
					break;
				}

				glGenTextures( 1, &textureID );
				glBindTexture( GL_TEXTURE_2D, textureID );

				SetTextureFilter( tf );

				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

				glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

				glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data );
				glBindTexture( GL_TEXTURE_2D, 0 );

				this->width = width;
				this->height = height;
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
				if ( tf == TextureFilter::NEAREST )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				}
				else if ( tf == TextureFilter::LINEAR )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				}
			}

			void OpenGLTexture::SetWrapped( bool wrapped )
			{
				glBindTexture( GL_TEXTURE_2D, textureID );

				if ( wrapped )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
				}
				else
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
				}
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
