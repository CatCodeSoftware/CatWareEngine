#pragma once

#include "CatWare/Graphics/Renderer/Buffer.h"
#include <glad/glad.h>

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			// temporary function for converting the ShaderDataType to a GLEnum
			inline unsigned int ShaderTypeToGLEnum( ShaderDataType dataType )
			{
				switch ( dataType )
				{
				case ShaderDataType::Float: return GL_FLOAT;
				case ShaderDataType::Float2: return GL_FLOAT;
				case ShaderDataType::Float3: return GL_FLOAT;
				case ShaderDataType::Float4: return GL_FLOAT;
				case ShaderDataType::Int: return GL_INT;
				case ShaderDataType::Int2: return GL_INT;
				case ShaderDataType::Int3: return GL_INT;
				case ShaderDataType::Int4: return GL_INT;
				case ShaderDataType::Bool: return GL_INT;
				}
			}

			class OpenGLVertexBuffer : public VertexBuffer
			{
			public:
				OpenGLVertexBuffer( unsigned int size, float* vertecies );
				virtual ~OpenGLVertexBuffer( );

				virtual void Bind( ) override;
				virtual void Unbind( ) override;

				virtual void SetLayout( BufferLayout layout ) override;

			private:
				unsigned int id;

				BufferLayout bufferLayout;
			};

			class OpenGLIndexBuffer : public IndexBuffer
			{
			public:
				OpenGLIndexBuffer( unsigned int count, unsigned int* indicies );
				virtual ~OpenGLIndexBuffer( );

				virtual void Bind( ) override;
				virtual void Unbind( ) override;

				virtual unsigned int GetCount( ) override;

			private:
				unsigned int count;
				unsigned int id;
			};
		}
	}
}