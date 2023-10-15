#pragma once

#include "CatWare/Core.h"
#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"
#include "CatWare/Utils/Vector.h"
#include "Shader.h"

namespace CatWare
{
	namespace Rendering
	{
		class CATWARE_API Renderer
		{
		public:
			static void Init( RendererAPI* a_rendererAPI );

			static void SetScreenSize( unsigned int width, unsigned int height );

			static void DrawRect( Vector2D position, Vector2D size, Color color );
			static void DrawRectTextured( Vector2D position, Vector2D size, Color tint );

			static void Clear( Color clearColor );

			static RendererAPI::API GetAPI( ) { return rendererAPI->GetRenderAPI( ); }

		private:
			static RendererAPI* rendererAPI;

			static Shader* rectShader;

			static unsigned int width;
			static unsigned int height;

			static float ScreenCoordToGLCoord( int screenCoord, int screenSize );
		};
	}
}