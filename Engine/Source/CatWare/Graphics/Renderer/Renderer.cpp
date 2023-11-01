#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CatWare/Debug/Debug.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "CatWare/Utils/Log.h"


using namespace CatWare::Rendering;

namespace CatWare
{
	RendererAPI* Renderer::rendererAPI = nullptr;

	Shader* Renderer::rectShader = nullptr;
	Shader* Renderer::rectTexturedShader = nullptr;
	Shader* Renderer::postProcessShader = nullptr;
	Shader* Renderer::textShader = nullptr;

	FrameBuffer* Renderer::currentFrameBuffer = nullptr;
	FrameBuffer* Renderer::defaultFrameBuffer = nullptr;

	Vector2D Renderer::renderOffset = { 0, 0 };

	unsigned int Renderer::width = 0;
	unsigned int Renderer::height = 0;

	void Renderer::Init( RendererAPI* a_rendererAPI )
	{
		CW_ENGINE_LOG->Info( "Initializing renderer" );

		rendererAPI = a_rendererAPI;

		rendererAPI->SetBlendAlpha( true );

		// Load shaders from EngineRes
		rectShader = Shader::CreateFromFile( "EngineRes/Shaders/RectShaderVertex.glsl", "EngineRes/Shaders/RectShaderFragment.glsl" );
		rectTexturedShader = Shader::CreateFromFile( "EngineRes/Shaders/RectTexturedVertex.glsl", "EngineRes/Shaders/RectTexturedFragment.glsl" );
		postProcessShader = Shader::CreateFromFile( "EngineRes/Shaders/PostProcessVertex.glsl", "EngineRes/Shaders/PostProcessFragment.glsl" );
		textShader = Shader::CreateFromFile( "EngineRes/Shaders/TextVertex.glsl", "EngineRes/Shaders/TextFragment.glsl" );

		// create framebuffer
		FrameBufferSpec fbSpec;
		fbSpec.width = width;
		fbSpec.height = height;

		defaultFrameBuffer = FrameBuffer::Create( fbSpec );
		SetRenderTarget( defaultFrameBuffer );
	}

	void Renderer::DeInit( )
	{
		delete rectShader;
		delete rectTexturedShader;
		delete postProcessShader;
		delete textShader;

		delete defaultFrameBuffer;
	}

	void Renderer::StartDrawing( )
	{
		SetRenderTarget( defaultFrameBuffer );
	}

	void Renderer::EndDrawing( )
	{
		currentFrameBuffer->Unbind( );
		
		float vertecies[4 * 4] =
		{
			-1, -1, 0, 0,
			1, -1, 1, 0,
			-1, 1, 0, 1,
			1, 1, 1, 1
		};

		unsigned int indicies[6] =
		{
			0, 1, 2, 2, 1, 3
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout =
		{
			BufferElement( "position", ShaderDataType::Float2 ),
			BufferElement( "textureCoord", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		postProcessShader->Bind( );

		currentFrameBuffer->GetColorAttachment( )->Bind( 0 );
		postProcessShader->SetUniform1f( "u_Texture", 0 );

		rendererAPI->DrawIndexed( vertexArray );

		delete vertexArray;
		delete vertexBuffer;
		delete indexBuffer;
	}

	void Renderer::SetRenderTarget( FrameBuffer* frameBuffer )
	{
		if ( currentFrameBuffer != nullptr )
			currentFrameBuffer->Unbind( );

		if ( frameBuffer == nullptr )
			currentFrameBuffer = defaultFrameBuffer;

		else
			currentFrameBuffer = frameBuffer;

		currentFrameBuffer->Bind( );
	}

	void Renderer::SetScreenSize( unsigned int a_width, unsigned int a_height )
	{
		width = a_width;
		height = a_height;

		// rendererAPI->SetViewportSize( width, height );
	}



	void Renderer::Clear( Color color )
	{
		rendererAPI->SetClearColor( color );
		rendererAPI->Clear( );
	}



	void Renderer::DrawRect( Vector2D position, Vector2D size, Color color, float rotation )
	{
		glm::mat4 projectionMatrix = glm::ortho( 0.0f - renderOffset.x, float( currentFrameBuffer->GetColorAttachment( )->GetTextureWidth( ) ) - renderOffset.x, float( currentFrameBuffer->GetColorAttachment( )->GetTextureHeight( ) ) - renderOffset.y, 0.0f - renderOffset.y );

		glm::mat4 transformMatrix = glm::mat4( 1.0f );

		transformMatrix = glm::translate( transformMatrix, glm::vec3( position.x + size.x / 2, position.y + size.y / 2, 0.0f ) );
		transformMatrix = glm::rotate( transformMatrix, glm::radians( rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
		transformMatrix = glm::translate( transformMatrix, glm::vec3( -( position.x + size.x / 2 ), -( position.y + size.y / 2 ), 0.0f ) );
		
		float vertecies[2 * 4] =
		{
			position.x, position.y,
			position.x + size.x, position.y,
			position.x, position.y + size.y,
			position.x + size.x, position.y + size.y
		};

		unsigned int indicies[6] =
		{
			0, 1, 2, 2, 1, 3
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout =
		{
			BufferElement( "position", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		rectShader->Bind( );
		rectShader->SetUniform4f( "u_Color", float( color.r ) / 255.0f, float( color.g ) / 255.0f, float( color.b ) / 255.0f, float( color.a ) / 255.0f );
		rectShader->SetUniformMat4( "u_Projection", projectionMatrix );
		rectShader->SetUniformMat4( "u_Transform", transformMatrix );

		rendererAPI->DrawIndexed( vertexArray );

		delete vertexArray;
		delete vertexBuffer;
		delete indexBuffer;
	}

	void Renderer::DrawRectTextured( Vector2D position, Vector2D size, Texture2D* texture, Color tint, float rotation )
	{
		glm::mat4 projectionMatrix = glm::ortho( 0.0f - renderOffset.x, float( currentFrameBuffer->GetColorAttachment( )->GetTextureWidth( ) ) - renderOffset.x, float( currentFrameBuffer->GetColorAttachment( )->GetTextureHeight( ) ) - renderOffset.y, 0.0f - renderOffset.y );

		glm::mat4 transformMatrix = glm::mat4( 1.0f );

		transformMatrix = glm::translate( transformMatrix, glm::vec3( position.x + size.x / 2, position.y + size.y / 2, 0.0f ) );
		transformMatrix = glm::rotate( transformMatrix, glm::radians( rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
		transformMatrix = glm::translate( transformMatrix, glm::vec3( -( position.x + size.x / 2 ), -( position.y + size.y / 2 ), 0.0f ) );

		float vertecies[4 * 4] =
		{
			position.x, position.y, 0, 0,
			position.x + size.x, position.y, 1, 0,
			position.x, position.y + size.y, 0, 1,
			position.x + size.x, position.y + size.y, 1 ,1
		};

		unsigned int indicies[6] =
		{
			0, 1, 2, 2, 1, 3
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout =
		{
			BufferElement( "position", ShaderDataType::Float2 ),
			BufferElement( "textureCoord", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		rectTexturedShader->Bind( );
		rectTexturedShader->SetUniform4f( "u_Tint", float( tint.r ) / 255.0f, float( tint.g ) / 255.0f, float( tint.b ) / 255.0f, float( tint.a ) / 255.0f );
		rectShader->SetUniformMat4( "u_Projection", projectionMatrix );
		rectShader->SetUniformMat4( "u_Transform", transformMatrix );


		texture->Bind( 0 );
		rectTexturedShader->SetUniform1f( "u_Texture", 0 );

		rendererAPI->DrawIndexed( vertexArray );

		texture->Unbind( );

		delete vertexArray;
		delete vertexBuffer;
		delete indexBuffer;
	}

	void Renderer::DrawCharacter( Text::Character* character, Vector2D position, unsigned int size, Color color )
	{
		glm::mat4 projectionMatrix = glm::ortho( 0.0f - renderOffset.x, float( currentFrameBuffer->GetColorAttachment( )->GetTextureWidth( ) ) - renderOffset.x, float( currentFrameBuffer->GetColorAttachment( )->GetTextureHeight( ) ) - renderOffset.y, 0.0f - renderOffset.y );

		textShader->Bind( );
		textShader->SetUniform4f( "u_Color", float( color.r ) / 255.0f, float( color.g ) / 255.0f, float( color.b ) / 255.0f, float( color.a ) / 255.0f );
		textShader->SetUniform1i( "u_Texture", 0 );
		rectShader->SetUniformMat4( "u_Projection", projectionMatrix );

		float xpos = position.x + character->bearing.x * size;
		float ypos = position.y * size;

		float w = character->size.x * size;
		float h = character->size.y * size;

		float vertices[6 * 4] =
		{
			xpos, ypos + h, 0, 1,
			xpos, ypos, 0, 0,
			xpos + w, ypos, 1, 0,

			xpos, ypos + h, 0, 1,
			xpos + w, ypos, 1, 0,
			xpos + w, ypos + h, 1, 1
		};

		unsigned int indicies[6] =
		{
			0, 1, 2, 3, 4, 5
		};

		character->texture->Bind( 0 );

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertices ), vertices );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout =
		{
			BufferElement( "position", ShaderDataType::Float2 ),
			BufferElement( "textureCoord", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		rendererAPI->DrawIndexed( vertexArray );

		delete vertexArray;
		delete vertexBuffer;
		delete indexBuffer;
	}

	void Renderer::DrawString( std::string string, Vector2D position, unsigned int size, Text::Font* font, Color color )
	{
		double offset = 0;

		for ( char ch : string )
		{
			if ( ch == ' ' )
			{
				offset += font->spaceSize * size;
			}
			else if ( ch == '\t' )
			{
				offset += font->tabSize * size;
			}
			else
			{
				Text::Character* character = font->GetCharacter( ch );
				DrawCharacter( character, { position.x + offset, position.y + ( ( font->GetSize( ) - character->bearing.y ) * size ) }, size, color );

				offset += ( character->size.x + character->bearing.x ) * size;
			}
		}
	}
}