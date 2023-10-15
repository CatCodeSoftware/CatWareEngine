#include "Application.h"

#include <glad/glad.h>

#include "Graphics/Renderer/Buffer.h"
#include "Graphics/Renderer/OpenGL/OpenGLBuffer.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Renderer/Shader.h"

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

		Renderer::Init( new OpenGL::OpenGLAPI );

		PostInit( );

		glClearColor( 0.2, 0.2, 0.2, 1 );

		float vertecies[6 * 3] = {
			0.5, 0.5, 1.0f, 0.0, 0.0, 1.0,
			0.5, -0.5, 0.0f, 1.0, 0.0, 1.0,
			-0.5, 0.5, 0.0f, 0.0, 1.0, 1.0,
		};

		unsigned int indicies[3] = {
			1, 2, 3
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 2, indicies );

		BufferLayout layout = {
			BufferElement( "position", ShaderDataType::Float2 ),
			BufferElement( "color", ShaderDataType::Float4 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		std::string vertexShader = R"(
#version 460 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 vertexColor;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);//position;
	vertexColor = color;
}
)";

		std::string fragmentShader = R"(
#version 460 core
out vec4 FragColor;
in vec4 vertexColor;


void main()
{
	FragColor = vertexColor;
}
)";

		Shader* shader = Shader::Create(
			vertexShader,
			fragmentShader
		);

		shader->Bind( );

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