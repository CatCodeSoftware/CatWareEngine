#pragma once

#include "CatWare/Core.h"
#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"
#include "CatWare/Utils/Vector.h"
#include "Shader.h"
#include "Texture.h"
#include "../Text.h"
#include "../Camera.h"

namespace CatWare
{
	class CATWARE_API Renderer
	{
	public:
		static OrthoCamera* camera2D;

		static void Init( Rendering::RendererAPI* a_rendererAPI, int screenWidth, int screenHeight );
		static void DeInit( );

		static void SetScreenSize( unsigned int width, unsigned int height );

		static void StartDrawing( );
		static void EndDrawing( );

		static void SetRenderTarget( Rendering::FrameBuffer* frameBuffer );

		static void DrawRect( Vector2D position, Vector2D size, Color color, glm::mat4 transformMatrix );
		static void DrawRect( Vector2D position, Vector2D size, Color color, float rotation = 0 );

		static void DrawRectTextured( Vector2D position, Vector2D size, Rendering::Texture2D* texture, glm::mat4 transformMatrix, Color tint = { 255, 255, 255, 255 } );
		static void DrawRectTextured( Vector2D position, Vector2D size, Rendering::Texture2D* texture, Color tint = { 255, 255, 255, 255 }, float rotation = 0 );

		static void DrawCharacter( Text::Character* character, Vector2D position, unsigned int scale , Color color = { 0, 0, 0, 255 } );
		static void DrawString( std::string string, Vector2D position, unsigned int scale, Text::Font* font, Color color = { 0, 0, 0, 255 } );

		static void Clear( Color clearColor );

		static Rendering::RendererAPI::API GetAPI( ) { return rendererAPI->GetRenderAPI( ); }

	private:
		static Rendering::RendererAPI* rendererAPI;

		static Rendering::Shader* rectShader;
		static Rendering::Shader* rectTexturedShader;
		static Rendering::Shader* postProcessShader;
		static Rendering::Shader* textShader;

		static Rendering::FrameBuffer* currentFrameBuffer;
		static Rendering::FrameBuffer* defaultFrameBuffer;

		static Rendering::VertexBuffer* rectVerts;
		static Rendering::IndexBuffer* rectIndexes;
		static Rendering::VertexArray* rectArray;

		static unsigned int width;
		static unsigned int height;
	};
}