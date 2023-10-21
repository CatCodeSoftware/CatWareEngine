#include "Renderer.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "CatWare/Utils/Log.h"

namespace CatWare
{
	namespace Rendering
	{
		RendererAPI* Renderer::rendererAPI = nullptr;

		Shader* Renderer::rectShader = nullptr;
		Shader* Renderer::rectTexturedShader = nullptr;
		Shader* Renderer::postProcessShader = nullptr;

		FrameBuffer* Renderer::frameBuffer = nullptr;

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

			// create framebuffer
			FrameBufferSpec fbSpec;
			fbSpec.width = width;
			fbSpec.height = height;

			frameBuffer = FrameBuffer::Create( fbSpec );
		}

		void Renderer::StartDrawing( )
		{
			frameBuffer->Bind( );
		}

		void Renderer::EndDrawing( )
		{
			frameBuffer->Unbind( );

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

			frameBuffer->GetColorAttachment( )->Bind( 0 );
			postProcessShader->SetUniform1f( "u_Texture", 0 );

			rendererAPI->DrawIndexed( vertexArray );

			delete vertexArray;
			delete vertexBuffer;
			delete indexBuffer;
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



		void Renderer::DrawRect( Vector2D position, Vector2D size, Color color )
		{
			float vertecies[2 * 4] =
			{
				ScreenCoordToGLCoord( position.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y, ScreenAxis::Y ),
				ScreenCoordToGLCoord( position.x + size.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y, ScreenAxis::Y ),
				ScreenCoordToGLCoord( position.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y + size.y, ScreenAxis::Y ),
				ScreenCoordToGLCoord( position.x + size.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y + size.y, ScreenAxis::Y )
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

			rendererAPI->DrawIndexed( vertexArray );

			delete vertexArray;
			delete vertexBuffer;
			delete indexBuffer;
		}

		void Renderer::DrawRectTextured( Vector2D position, Vector2D size, Texture2D* texture, Color tint )
		{
			float vertecies[4 * 4] =
			{
				ScreenCoordToGLCoord( position.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y, ScreenAxis::Y ), 0, 0,
				ScreenCoordToGLCoord( position.x + size.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y, ScreenAxis::Y ), 1, 0,
				ScreenCoordToGLCoord( position.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y + size.y, ScreenAxis::Y ), 0, 1,
				ScreenCoordToGLCoord( position.x + size.x, ScreenAxis::X ), -ScreenCoordToGLCoord( position.y + size.y, ScreenAxis::Y ), 1, 1
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

			texture->Bind( 0 );
			rectTexturedShader->SetUniform1f( "u_Texture", 0 );

			rendererAPI->DrawIndexed( vertexArray );

			delete vertexArray;
			delete vertexBuffer;
			delete indexBuffer;
		}


		float Renderer::ScreenCoordToGLCoord( int screenCoord, ScreenAxis axis )
		{
			unsigned int screenSize = 0;
			unsigned int offset = 0;

			if ( axis == ScreenAxis::X )
			{
				screenSize = width;
				offset = renderOffset.x;
			}
			else
			{
				screenSize = height;
				offset = renderOffset.y;
			}

			return ( ( float( screenCoord + offset ) / float( screenSize ) ) * 2 - 1 );
		}
	}
}