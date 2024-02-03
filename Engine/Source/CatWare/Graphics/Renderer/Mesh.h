#pragma once
#include "Buffer.h"

namespace CatWare
{
	class Mesh
	{
	public:
		Mesh( Rendering::VertexBuffer* vertexBuffer, Rendering::IndexBuffer* indexBuffer );
		~Mesh( );

		inline Rendering::VertexArray* GetVertexArray( ) { return vertexArray; }

	private:
		Rendering::VertexBuffer* vertexBuffer;
		Rendering::IndexBuffer* indexBuffer;
		Rendering::VertexArray* vertexArray;
	};
}