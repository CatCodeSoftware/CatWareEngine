#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			// Vertex
			OpenGLVertexBuffer::OpenGLVertexBuffer( unsigned int size, float* vertecies )
			{
				glCreateBuffers( 1, &id );
				Bind( );
				glBufferData( GL_ARRAY_BUFFER, size, vertecies, GL_STATIC_DRAW );
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

				unsigned int index = 0;
				for ( BufferElement& element : bufferLayout.GetBufferElements( ) )
				{
					glEnableVertexAttribArray( index );
					glVertexAttribPointer( index, element.GetComponentCount( ), OpenGL::ShaderTypeToGLEnum( element.dataType ), false, bufferLayout.GetStride( ), ( void* ) element.offset );
					index++;
				}
			}


			// Index

			OpenGLIndexBuffer::OpenGLIndexBuffer( unsigned int count, unsigned int* indicies )
			{
				glCreateBuffers( 1, &id );
				Bind( );
				glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), indicies, GL_STATIC_DRAW );

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
		}
	}
}