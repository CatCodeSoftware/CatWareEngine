#include "Buffer.h"

#include "RendererAPI.h"
#include "Renderer.h"
#include "OpenGL/OpenGLBuffer.h"

namespace CatWare
{
	namespace Rendering
	{
		// ---------------------------------------
		// BufferElements ------------------------
		// ---------------------------------------

		BufferElement::BufferElement( std::string name, ShaderDataType dataType )
		{
			this->name = name;
			this->dataType = dataType;

			size = GetShaderDataTypeSize( dataType );
			offset = 0;
		}

		unsigned int BufferElement::GetComponentCount( )
		{
			switch ( dataType )
			{
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;
			case ShaderDataType::Bool: return 1;
			}

			return 0;
		}

		// ---------------------------------------
		// BufferLayout --------------------------
		// ---------------------------------------

		BufferLayout::BufferLayout( )
		{

		}

		BufferLayout::BufferLayout( std::initializer_list<BufferElement> elements )
		{
			this->bufferElements = elements;

			CalculateOffsetsAndStride( );
		}

		void BufferLayout::CalculateOffsetsAndStride( )
		{
			unsigned int offset = 0;
			stride = 0;

			for ( BufferElement& element : bufferElements )
			{
				element.offset = offset;
				element.count = element.GetComponentCount( );
				offset += element.size;
				stride += element.size;	
			}
		}

		// ---------------------------------------
		// VertexBuffer --------------------------
		// ---------------------------------------

		VertexBuffer* VertexBuffer::Create( unsigned int size, float* vertecies )
		{
			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::NONE:
					return nullptr;
					break;

			case RendererAPI::API::OPENGL:
				return new OpenGL::OpenGLVertexBuffer( size, vertecies );

			}

			return nullptr;
		}

		// ---------------------------------------
		// IndexBuffer ---------------------------
		// ---------------------------------------

		IndexBuffer* IndexBuffer::Create( unsigned int count, unsigned int* indicies )
		{
			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::NONE:
				return nullptr;
				break;

			case RendererAPI::API::OPENGL:
				return new OpenGL::OpenGLIndexBuffer( count, indicies );
			}

			return nullptr;
		}


		// ---------------------------------------
		// VertexArray ---------------------------
		// ---------------------------------------
		VertexArray* VertexArray::Create( )
		{
			switch ( Renderer::GetAPI( ) )
			{
			case RendererAPI::API::OPENGL:
				return new OpenGL::OpenGLVertexArray;
			}
		}
	}
}
