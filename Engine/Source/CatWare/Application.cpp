#include "Application.h"

#include <glad/glad.h>
#include <stb_image.h>


#include "Graphics/Renderer/Buffer.h"
#include "Graphics/Renderer/OpenGL/OpenGLBuffer.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/Texture.h"

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

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );

		float vertecies[4 * 4] = {
			1, 1, 1, 1,
			1, -1, 1, 0,
			-1, 1, 0, 1,
			-1, -1, 0, 0
		};

		unsigned int indicies[6] = {
			0, 1, 2, 2, 3, 1
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout = {
			BufferElement( "position", ShaderDataType::Float2 ),
			BufferElement( "uv_coords", ShaderDataType::Float2 )
		};

		stbi_set_flip_vertically_on_load( true );

		int width;
		int height;
		void* rgbaData = stbi_load( "cat.png", &width, &height, nullptr, 4 );

		Texture2D* texture = Texture2D::Create( width, height, rgbaData );
		texture->Bind( 0 );

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		std::string vertexShader = R"(
#version 460 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

void main()
{
	v_texCoord = texCoord;
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);//position;
}
)";

		std::string fragmentShader = R"(
#version 460 core

uniform sampler2D texture2d;
in vec2 v_texCoord;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture2d, v_texCoord);
}
)";

		Shader* shader = Shader::Create(
			vertexShader,
			fragmentShader
		);

		shader->Bind( );

		shader->SetUniform1i( "texture2d", 0 );

		while ( running )
		{
			window->HandleWindowEvents( );
			running = !window->ShouldClose( );

			glClear( GL_COLOR_BUFFER_BIT );

			glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

			window->SwapBuffers( );
		}

		delete window;
	}
}