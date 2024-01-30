#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CatWare/Debug/Debug.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "CatWare/Error.h"
#include "CatWare/Log.h"
#include "CatWare/Debug/Console.h"
#include "CatWare/Types/Transform.h"

using namespace CatWare::Rendering;

namespace CatWare
{
	void Renderer::Init( RendererAPI* a_rendererAPI, int screenWidth, int screenHeight )
	{
		CW_ENGINE_LOG->Info( "Initializing renderer" );

		rendererAPI = a_rendererAPI;

		if ( rendererAPI == nullptr )
			CW_ABORT( "rendererAPI was nullptr" );
		else if ( rendererAPI->GetRenderAPI( ) == RendererAPI::API::NONE )
			CW_ABORT( "Renderer API was NONE" );

		rendererAPI->SetBlendAlpha( true );

		// Load shaders from EngineRes
		rectShader = Shader::CreateFromFile( "EngineRes/Shaders/RectVertex.glsl",
		                                     "EngineRes/Shaders/RectFragment.glsl" );
		postProcessShader = Shader::CreateFromFile( "EngineRes/Shaders/PostProcessVertex.glsl",
		                                            "EngineRes/Shaders/PostProcessFragment.glsl" );
		textShader = Shader::CreateFromFile( "EngineRes/Shaders/TextVertex.glsl",
		                                     "EngineRes/Shaders/TextFragment.glsl" );

		width = screenWidth;
		height = screenHeight;

		// create framebuffer
		FrameBufferSpec fbSpec;
		fbSpec.width = width;
		fbSpec.height = height;

		defaultFrameBuffer = FrameBuffer::Create( fbSpec );
		SetRenderTarget( defaultFrameBuffer );

		camera2D = new OrthoCamera( screenWidth, screenHeight );

		// Create basic geometry for the rectangles
		float rectVerteciesTemp[] = {
			0 , 0 , 0 , 0 ,
			1 , 0 , 1 , 0 ,
			0 , 1 , 0 , 1 ,
			1 , 1 , 1 , 1
		};

		unsigned int rectIndiciesTemp[6] =
		{
			0 , 1 , 2 , 2 , 1 , 3
		};

		Rendering::VertexBuffer* rectVerts = Rendering::VertexBuffer::Create( sizeof( rectVerteciesTemp ), rectVerteciesTemp );

		BufferLayout vertBufferLayout =
		{
			BufferElement( "position", ShaderDataType::Float2 ) ,
			BufferElement( "textureCoord", ShaderDataType::Float2 )
		};

		rectVerts->SetLayout( vertBufferLayout );


		Rendering::IndexBuffer* rectIndexes = Rendering::IndexBuffer::Create( 6, rectIndiciesTemp );

		rectMesh = new Mesh( rectVerts, rectIndexes );


		Console::RegisterConVar( "r_pp_brightness", ConVar( ConVarType::FLOAT, &postProcess.brightness ) );
		Console::RegisterConVar( "r_pp_contrast", ConVar( ConVarType::FLOAT, &postProcess.contrast ) );
		Console::RegisterConVar( "r_pp_exposure", ConVar( ConVarType::FLOAT, &postProcess.exposure ) );
		Console::RegisterConVar( "r_pp_saturation", ConVar( ConVarType::FLOAT, &postProcess.saturation ) );
		Console::RegisterConVar( "r_pp_sharpness", ConVar( ConVarType::FLOAT, &postProcess.sharpness ) );
	}

	void Renderer::DeInit( )
	{
		delete rectMesh;

		delete rectShader;
		delete postProcessShader;
		delete textShader;
		delete camera2D;
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
			-1 , -1 , 0 , 0 ,
			1 , -1 , 1 , 0 ,
			-1 , 1 , 0 , 1 ,
			1 , 1 , 1 , 1
		};

		unsigned int indicies[6] =
		{
			0 , 1 , 2 , 2 , 1 , 3
		};

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertecies ), vertecies );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout =
		{
			BufferElement( "position", ShaderDataType::Float2 ) ,
			BufferElement( "textureCoord", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		postProcessShader->Bind( );

		postProcessShader->SetUniform2f( "u_Resolution", width, height );

		currentFrameBuffer->GetColorAttachment( )->Bind( 0 );
		postProcessShader->SetUniform1i( "u_Texture", 0 );

		postProcessShader->SetUniform1f( "u_Brightness", postProcess.brightness );
		postProcessShader->SetUniform1f( "u_Contrast", postProcess.contrast );
		postProcessShader->SetUniform1f( "u_Exposure", postProcess.exposure );
		postProcessShader->SetUniform1f( "u_Saturation", postProcess.saturation );
		postProcessShader->SetUniform1f( "u_Sharpness", postProcess.sharpness );
		postProcessShader->SetUniform4f( "u_Tint", float( postProcess.tint.r ) / 255.0f, float( postProcess.tint.g ) / 255.0f, float( postProcess.tint.b ) / 255.0f, float( postProcess.tint.a ) / 255.0f );

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

		delete camera2D;
		camera2D = new OrthoCamera( a_width, a_height );

		rendererAPI->SetViewportSize( width, height );
	}


	void Renderer::Clear( Color color )
	{
		rendererAPI->SetClearColor( color );
		rendererAPI->Clear( );
	}

	void Renderer::SubmitMesh( Mesh* mesh )
	{
		rendererAPI->DrawIndexed( mesh->GetVertexArray( ) );
	}

	void Renderer::DrawRect( Vector2D position, Vector2D size, Color color, glm::mat4 transformMatrix )
	{
		Vector2D renderOffset = camera2D->GetOffset( );

		if ( ( position.x + renderOffset.x < 0 - size.x * 2 || position.x  + renderOffset.x > width + size.x * 2 ) || (
				 position.y  + renderOffset.y < 0 - size.y * 2 || position.y  + renderOffset.y > height + size.y * 2 ) )
		{
			return;
		}

		glm::mat4 projectionMatrix = camera2D->CalculateProjectionMatrix( );

		position = position / size;

		rectShader->Bind( );

		rectShader->SetUniform2f( "u_Size", float( size.x ), float( size.y ) );
		rectShader->SetUniform2f( "u_Position", position.x, position.y );

		rectShader->SetUniform1i( "u_IsTextured", false );
		rectShader->SetUniform4f( "u_Tint", float( color.r ) / 255.0f, float( color.g ) / 255.0f,
		                          float( color.b ) / 255.0f, float( color.a ) / 255.0f );
		rectShader->SetUniformMat4( "u_Projection", projectionMatrix );
		rectShader->SetUniformMat4( "u_Transform", transformMatrix );

		SubmitMesh( rectMesh );
	}

	void Renderer::DrawRect( Vector2D position, Vector2D size, Color color, float rotation )
	{
		glm::mat4 transformMatrix = glm::mat4( 1.0f );

		transformMatrix = glm::translate( transformMatrix,
		                                  glm::vec3( position.x + size.x / 2, position.y + size.y / 2, 0.0f ) );
		transformMatrix = glm::rotate( transformMatrix, glm::radians( rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
		transformMatrix = glm::translate( transformMatrix,
		                                  glm::vec3( -( position.x + size.x / 2 ), -( position.y + size.y / 2 ),
		                                  0.0f ) );

		DrawRect( position, size, color, transformMatrix );
	}

	void Renderer::DrawRect( Transform transform, Color color )
	{
		DrawRect( transform.position, transform.size, color, transform.rotation );
	}

	void Renderer::DrawRectTextured( Vector2D position, Vector2D size, Rendering::Texture2D* texture,
	                                 glm::mat4 transformMatrix, Color tint )
	{
		if ( texture == nullptr )
			CW_ABORT( "texture was nullptr" );

		Vector2D renderOffset = camera2D->GetOffset( );

		if ( ( position.x + renderOffset.x < 0 - size.x * 2 || position.x  + renderOffset.x > width + size.x * 2 ) || (
			     position.y  + renderOffset.y < 0 - size.y * 2 || position.y  + renderOffset.y > height + size.y * 2 ) )
		{
			return;
		}

		glm::mat4 projectionMatrix = camera2D->CalculateProjectionMatrix( );

		position = position / size;

		rectShader->Bind( );
		rectShader->SetUniform4f( "u_Tint", float( tint.r ) / 255.0f, float( tint.g ) / 255.0f,
		                          float( tint.b ) / 255.0f, float( tint.a ) / 255.0f );

		rectShader->SetUniform2f( "u_Size", size.x, size.y );
		rectShader->SetUniform2f( "u_Position", position.x, position.y );

		rectShader->SetUniformMat4( "u_Projection", projectionMatrix );
		rectShader->SetUniformMat4( "u_Transform", transformMatrix );


		texture->Bind( 0 );
		rectShader->SetUniform1i( "u_IsTextured", true );
		rectShader->SetUniform1f( "u_Texture", 0 );

		SubmitMesh( rectMesh );

		texture->Unbind( );
	}

	void Renderer::DrawRectTextured( Vector2D position, Vector2D size, Texture2D* texture, Color tint, float rotation )
	{
		glm::mat4 transformMatrix = glm::mat4( 1.0f );

		transformMatrix = glm::translate( transformMatrix,
		                                  glm::vec3( position.x + size.x / 2, position.y + size.y / 2, 0.0f ) );
		transformMatrix = glm::rotate( transformMatrix, glm::radians( rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
		transformMatrix = glm::translate( transformMatrix,
		                                  glm::vec3( -( position.x + size.x / 2 ), -( position.y + size.y / 2 ),
		                                             0.0f ) );

		DrawRectTextured( position, size, texture, transformMatrix, tint );
	}

	void Renderer::DrawRectTextured( Transform transform, Rendering::Texture2D* texture, Color tint )
	{
		DrawRectTextured( transform.position, transform.size, texture, tint, transform.rotation );
	}

	void Renderer::DrawCharacter( Text::Character* character, Vector2D position, unsigned int size, Color color )
	{
		glm::mat4 projectionMatrix = camera2D->CalculateProjectionMatrix( );

		textShader->Bind( );
		textShader->SetUniform4f( "u_Color", float( color.r ) / 255.0f, float( color.g ) / 255.0f,
		                          float( color.b ) / 255.0f, float( color.a ) / 255.0f );
		textShader->SetUniform1i( "u_Texture", 0 );
		textShader->SetUniformMat4( "u_Projection", projectionMatrix );

		float xpos = position.x + character->bearing.x * size;
		float ypos = position.y * size;

		float w = character->size.x * size;
		float h = character->size.y * size;

		float vertices[6 * 4] =
		{
			xpos , ypos + h , 0 , 1 ,
			xpos , ypos , 0 , 0 ,
			xpos + w , ypos , 1 , 0 ,

			xpos , ypos + h , 0 , 1 ,
			xpos + w , ypos , 1 , 0 ,
			xpos + w , ypos + h , 1 , 1
		};

		unsigned int indicies[6] =
		{
			0 , 1 , 2 , 3 , 4 , 5
		};

		character->texture->Bind( 0 );

		VertexBuffer* vertexBuffer = VertexBuffer::Create( sizeof( vertices ), vertices );
		IndexBuffer* indexBuffer = IndexBuffer::Create( 6, indicies );

		BufferLayout layout =
		{
			BufferElement( "position", ShaderDataType::Float2 ) ,
			BufferElement( "textureCoord", ShaderDataType::Float2 )
		};

		vertexBuffer->SetLayout( layout );

		VertexArray* vertexArray = VertexArray::Create( );

		vertexArray->AddVertexBuffer( vertexBuffer );
		vertexArray->SetIndexBuffer( indexBuffer );

		rendererAPI->DrawIndexed( vertexArray );

		vertexBuffer->Unbind();
		indexBuffer->Unbind( );
		vertexArray->Unbind( );

		delete vertexArray;
		delete vertexBuffer;
		delete indexBuffer;
	}

	void Renderer::DrawString( std::string string, Vector2D position, unsigned int size, Text::Font* font, Color color )
	{
		double offset = 0;

		for ( char ch: string )
		{
			if ( ch == ' ' )
			{
				offset += font->spaceSize * size;
			} else if ( ch == '\t' )
			{
				offset += font->tabSize * size;
			} else
			{
				Text::Character* character = font->GetCharacter( ch );
				DrawCharacter( character, {
					               position.x + offset ,
					               position.y + ( ( font->GetSize( ) - character->bearing.y ) * size )
				               }, size, color );

				offset += ( character->size.x + character->bearing.x ) * size;
			}
		}
	}
}
