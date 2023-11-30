#include "OpenGLBuffer.h"

#include <glad/glad.h>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Log.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{

			// --------------------------
			// VertexBuffer -------------
			// --------------------------

			OpenGLVertexBuffer::OpenGLVertexBuffer( unsigned int size, float* vertecies )
			{
				glCreateBuffers( 1, &id );
				Bind( );
				glBufferData( GL_ARRAY_BUFFER, size, vertecies, GL_STREAM_DRAW );
			}

			OpenGLVertexBuffer::~OpenGLVertexBuffer( )
			{
				glDeleteBuffers( 1, &id );
			}

			void OpenGLVertexBuffer::Bind( )
			{
				glBindBuffer( GL_ARRAY_BUFFER, id );
			}

			void OpenGLVertexBuffer::Unbind( )
			{
				glBindBuffer( GL_ARRAY_BUFFER, 0 );
			}

			void OpenGLVertexBuffer::SetLayout( BufferLayout layout )
			{
				bufferLayout = layout;
			}

			BufferLayout OpenGLVertexBuffer::GetLayout( )
			{
				return bufferLayout;
			}

			// --------------------------
			// IndexBuffer --------------
			// --------------------------

			OpenGLIndexBuffer::OpenGLIndexBuffer( unsigned int count, unsigned int* indicies )
			{
				glCreateBuffers( 1, &id );
				Bind( );
				glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), indicies, GL_STREAM_DRAW );

				this->count = count;
			}

			OpenGLIndexBuffer::~OpenGLIndexBuffer( )
			{
				glDeleteBuffers( 1, &id );
			}

			void OpenGLIndexBuffer::Bind( )
			{
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );
			}

			void OpenGLIndexBuffer::Unbind( )
			{
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
			}

			unsigned int OpenGLIndexBuffer::GetCount( )
			{
				return count;
			}


			// --------------------------
			// VertexArray --------------
			// --------------------------

			OpenGLVertexArray::OpenGLVertexArray( )
			{
				glCreateVertexArrays( 1, &id );
			}

			OpenGLVertexArray::~OpenGLVertexArray( )
			{
				glDeleteVertexArrays( 1, &id );
			}

			void OpenGLVertexArray::Bind( )
			{
				glBindVertexArray( id );
			}

			void OpenGLVertexArray::Unbind( )
			{
				glBindVertexArray( 0 );
			}

			void OpenGLVertexArray::AddVertexBuffer( VertexBuffer* vertexBuffer )
			{
				Bind( );
				vertexBuffer->Bind( );

				BufferLayout bufferLayout = vertexBuffer->GetLayout( );

				unsigned int index = 0;
				for ( BufferElement& element : bufferLayout.GetBufferElements( ) )
				{
					glEnableVertexAttribArray( index );
					glVertexAttribPointer( index, element.GetComponentCount( ), OpenGL::ShaderTypeToGLEnum( element.dataType ), false, bufferLayout.GetStride( ), ( void* ) element.offset );
					index++;
				}

				vertexBuffers.push_back( vertexBuffer );
			}

			void OpenGLVertexArray::SetIndexBuffer( IndexBuffer* indexBuffer )
			{
				Bind( );
				indexBuffer->Bind( );

				this->indexBuffer = indexBuffer;
			}

			IndexBuffer* OpenGLVertexArray::GetIndexBuffer( )
			{
				return indexBuffer;
			}

			// --------------------------
			// FRAMEBUFFER --------------
			// --------------------------

			OpenGLFrameBuffer::OpenGLFrameBuffer( const FrameBufferSpec& spec )
			{
				this->frameBufferSpec = spec;

				glCreateFramebuffers( 1, &frameBufferID );
				Bind( );
				
				colorAtachment = Texture2D::Create( spec.width, spec.height, nullptr );
				colorAtachment->Unbind( );

				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAtachment->GetTextureID( ), 0 );

				if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
				{
					CW_ENGINE_LOG->Error( "Broken framebuffer!" );
				}

				Unbind( );
			}

			OpenGLFrameBuffer::~OpenGLFrameBuffer( )
			{
				glDeleteTextures( 1, &depthAttachment );
				delete colorAtachment;
				glDeleteFramebuffers( 1, &frameBufferID );
			}

			void OpenGLFrameBuffer::Bind( )
			{
				glBindFramebuffer( GL_FRAMEBUFFER, frameBufferID );
			}

			void OpenGLFrameBuffer::Unbind( )
			{
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );
			}

			FrameBufferSpec OpenGLFrameBuffer::GetSpecification( )
			{
				return frameBufferSpec;
			}
}
	}
}