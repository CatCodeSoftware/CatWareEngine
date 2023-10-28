#pragma once

#include "CatWare/Core.h"
#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"
#include "CatWare/Utils/Vector.h"
#include "Shader.h"
#include "Texture.h"
#include "../Text.h"

namespace CatWare
{
	class CATWARE_API Renderer
	{
	public:
		static Vector2D renderOffset;

		static void Init( Rendering::RendererAPI* a_rendererAPI );

		static void SetScreenSize( unsigned int width, unsigned int height );

		static void StartDrawing( );
		static void EndDrawing( );

		static void DrawRect( Vector2D position, Vector2D size, Color color );
		static void DrawRectTextured( Vector2D position, Vector2D size, Rendering::Texture2D* texture, Color tint );

		static void DrawCharacter( Text::Character* character, Vector2D position, unsigned int scale , Color color = { 0, 0, 0, 255 } );
		static void DrawString( std::string string, Vector2D position, unsigned int scale, Text::Font* font, Color color = { 0, 0, 0, 255 } );

		static void DrawRectRotated( Vector2D position, Vector2D size, Color color, float rotation );
		static void DrawRectTexturedRotated( );

		static void Clear( Color clearColor );

		static Rendering::RendererAPI::API GetAPI( ) { return rendererAPI->GetRenderAPI( ); }

	private:
		enum class ScreenAxis
		{
			X, Y
		};

		static Rendering::RendererAPI* rendererAPI;

		static Rendering::Shader* rectShader;
		static Rendering::Shader* rectTexturedShader;
		static Rendering::Shader* postProcessShader;
		static Rendering::Shader* textShader;

		static Rendering::FrameBuffer* frameBuffer;

		static unsigned int width;
		static unsigned int height;

		static float TransformCoord( int screenCoord, ScreenAxis axis, float rotation = 0 );
	};
}