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

		glClearColor( 0.2, 0.2, 0.2, 1 );

		float vertecies[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f
		};

		unsigned int indicies[6] = { 0, 1, 2, 2, 3, 1 };

		Rendering::VertexBuffer* vertexBuffer = Rendering::VertexBuffer::Create( sizeof( vertecies ), vertecies  );

		BufferLayout bufferLayout =
		{
			BufferElement( "position", ShaderDataType::Float3 )
		};

		unsigned int vao;
		glCreateVertexArrays( 1, &vao );
		glBindVertexArray( vao );

		vertexBuffer->SetLayout( bufferLayout );

		Rendering::IndexBuffer* indexBuffer = Rendering::IndexBuffer::Create( 6, indicies );

		vertexBuffer->Bind( );
		indexBuffer->Bind( );

		PostInit( );

		while ( running )
		{
			window->HandleWindowEvents( );

			running = !window->ShouldClose( );

			glClear( GL_COLOR_BUFFER_BIT );
			glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

			window->SwapBuffers( );
		}
	}
}