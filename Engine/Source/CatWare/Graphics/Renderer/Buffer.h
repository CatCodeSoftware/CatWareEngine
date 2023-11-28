#pragma once

#include <vector>
#include <string>

#include "CatWare/Types/Types.h"
#include "Texture.h"
#include "CatWare/Core.h"

namespace CatWare
{
	namespace Rendering
	{
		enum class ShaderDataType
		{
			None = 0,
			Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool
		};

		inline unsigned int GetShaderDataTypeSize( ShaderDataType type )
		{
			switch ( type )
			{
			case ShaderDataType::Float: return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;
			case ShaderDataType::Int: return 4;
			case ShaderDataType::Int2: return 4 * 2;
			case ShaderDataType::Int3: return 4 * 3;
			case ShaderDataType::Int4: return 4 * 4;
			case ShaderDataType::Bool: return 1;
			}

			return 0;
		}

		class BufferElement
		{
		public:
			std::string name;
			unsigned int offset;
			unsigned int size;
			unsigned int count;

			ShaderDataType dataType;

			BufferElement( std::string name, ShaderDataType dataType );

			unsigned int GetComponentCount( );
		};

		class BufferLayout
		{
		public:
			BufferLayout( );
			BufferLayout( std::initializer_list<BufferElement> elements );

			inline std::vector<BufferElement>& GetBufferElements( ) { return bufferElements; }
			inline unsigned int GetStride( ) { return stride; }

		private:
			void CalculateOffsetsAndStride( );

			std::vector<BufferElement> bufferElements;
			unsigned int stride = 0;
		};

		class VertexBuffer
		{
		public:
			virtual ~VertexBuffer( ) {}

			virtual void Bind( ) = 0;
			virtual void Unbind( ) = 0;
			
			virtual void SetLayout( BufferLayout layout ) = 0;
			virtual BufferLayout GetLayout( ) = 0;

			static VertexBuffer* Create( unsigned int size, float* vertecies );
		};

		class IndexBuffer
		{
		public:
			virtual ~IndexBuffer( ) {}

			virtual void Bind( ) = 0;
			virtual void Unbind( ) = 0;

			virtual unsigned int GetCount( ) = 0;

			static IndexBuffer* Create( unsigned int size, unsigned int* indicies );
		};

		class VertexArray
		{
		public:
			virtual ~VertexArray( ) {}

			virtual void Bind( ) = 0;
			virtual void Unbind( ) = 0;

			virtual void AddVertexBuffer( VertexBuffer* vertexBuffer ) = 0;
			virtual void SetIndexBuffer( IndexBuffer* indexBuffer ) = 0;

			virtual IndexBuffer* GetIndexBuffer( ) = 0;

			static VertexArray* Create( );
		};


		class FrameBufferSpec
		{
		public:
			UInt32 width, height;
			UInt32 samples = 1;
		};

		class FrameBuffer
		{
		public:
			virtual ~FrameBuffer( ) {}

			virtual FrameBufferSpec GetSpecification( ) = 0;

			virtual void Bind( ) = 0;
			virtual void Unbind( ) = 0;

			virtual Texture2D* GetColorAttachment( ) = 0;

			CATWARE_API static FrameBuffer* Create( const FrameBufferSpec& spec );

		private:
		};
	}
}