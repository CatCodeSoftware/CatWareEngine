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
	namespace Rendering
	{
		class CATWARE_API Renderer
		{
		public:
			static Vector2D renderOffset;

			static void Init( RendererAPI* a_rendererAPI );

			static void SetScreenSize( unsigned int width, unsigned int height, bool resizeViewport = true );

			static void StartDrawing( );
			static void EndDrawing( );

			static void DrawRect( Vector2D position, Vector2D size, Color color );
			static void DrawRectTextured( Vector2D position, Vector2D size, Texture2D* texture, Color tint );

			static void DrawCharacter( Text::Character* character, Vector2D position, unsigned int scale , Color color = { 0, 0, 0, 255 } );
			static void DrawString( std::string string, Vector2D position, unsigned int scale, Text::Font* font, Color color = { 0, 0, 0, 255 } );

			static void Clear( Color clearColor );

			inline static Texture2D* GetViewportTexture( )
			{
				return frameBuffer->GetColorAttachment( );
			}

			static RendererAPI::API GetAPI( ) { return rendererAPI->GetRenderAPI( ); }

		private:
			enum class ScreenAxis
			{
				X, Y
			};

			static RendererAPI* rendererAPI;

			static Shader* rectShader;
			static Shader* rectTexturedShader;
			static Shader* postProcessShader;
			static Shader* textShader;

			static FrameBuffer* frameBuffer;

			static unsigned int width;
			static unsigned int height;

			static float ScreenCoordToGLCoord( int screenCoord, ScreenAxis axis );
		};
	}
}