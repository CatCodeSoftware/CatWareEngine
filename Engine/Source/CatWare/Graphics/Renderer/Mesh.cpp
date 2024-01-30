#include "Mesh.h"

namespace CatWare
{
	Mesh::Mesh( Rendering::VertexBuffer* vertexBuffer, Rendering::IndexBuffer* indexBuffer )
	{
		vertexArray = Rendering::VertexArray::Create( );

		this->vertexBuffer = vertexBuffer;
		this->indexBuffer = indexBuffer;

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );
	}

	Mesh::~Mesh( )
	{
		delete vertexArray;
		delete vertexBuffer;
		delete indexBuffer;
	}
}
