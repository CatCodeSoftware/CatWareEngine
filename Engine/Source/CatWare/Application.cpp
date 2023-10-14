#include "Application.h"

#include <glad/glad.h>

#include "Graphics/Renderer/Buffer.h"
#include "Graphics/Renderer/OpenGL/OpenGLBuffer.h"

namespace CatWare
{
	using namespace CatWare::Rendering;

	Application::Application( )
	{

	}

	Application::~Application( )
	{

	}

	void Application::Run( )
	{
		window = new Window( "CatWare", 1280, 720, false );

		PostInit( );

		glClearColor( 0.2, 0.2, 0.2, 1 );

		float vertecies[2 * 3] = {
			0.5, 0.5,
			0.5, -0.5,
			-0.5, 0.5,
		};

		unsigned int indicies[3] = {
			1, 2, 3
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 2, indicies );

		BufferLayout layout = {
			BufferElement( "position", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		while ( running )
		{
			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			glClear( GL_COLOR_BUFFER_BIT );

			glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );

			window->SwapBuffers( );
		}

		delete window;
	}
}