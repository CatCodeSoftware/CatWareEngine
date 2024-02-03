#pragma once

#include "../Camera.h"
#include "../Text.h"
#include "CatWare/Core.h"
#include "CatWare/Types/Transform.h"
#include "CatWare/Types/Vector.h"
#include "Mesh.h"
#include "OpenGL/OpenGLRendererAPI.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "Texture.h"

namespace CatWare
{
	class PostProcess
	{
	public:
		float brightness = 0;
		float contrast = 1;
		float exposure = 1;
		float saturation = 0;
		float sharpness = 0;

		Color tint;
	};

	class CATWARE_API Renderer
	{
	public:
		inline static OrthoCamera* camera2D = nullptr;

		inline static PostProcess postProcess;

		static void Init( Rendering::RendererAPI* a_rendererAPI, int screenWidth, int screenHeight );
		static void DeInit( );

		static void SetScreenSize( unsigned int width, unsigned int height );
		static inline Vector2D GetScreenSize( ) { return Vector2D( width, height ); }

		static void StartDrawing( );
		static void EndDrawing( );

		static void SetRenderTarget( Rendering::FrameBuffer* frameBuffer );
		static inline Rendering::FrameBuffer* GetRenderTarget( ) { return currentFrameBuffer; }

		static void SubmitMesh( Mesh* mesh, Rendering::Shader* shader, glm::mat4 transform );

		static void DrawRect( Vector2D position, Vector2D size, Color color, float rotation = 0, Vector2D rotationOrigin = { -1, -1 } );
		static void DrawRect( Transform transform, Color color );

		static void DrawRectTextured( Vector2D position, Vector2D size, Rendering::Texture2D* texture, Color tint = { 255, 255, 255, 255 }, float rotation = 0, bool wrapped = false );
		static void DrawRectTextured( Transform transform, Rendering::Texture2D* texture, Color tint = { 255, 255, 255, 255 }, bool wrapped = false	 );

		static void DrawLine( Vector2D begin, Vector2D end, Color color );

		static void DrawCharacter( Text::Character* character, Vector2D position, unsigned int scale , Color color = { 0, 0, 0, 255 } );
		static void DrawString( std::string string, Vector2D position, unsigned int scale, Text::Font* font, Color color = { 0, 0, 0, 255 } );

		static void Clear( Color clearColor );

		static Rendering::RendererAPI::API GetAPI( ) { return rendererAPI->GetRenderAPI( ); }

	private:
		inline static Rendering::RendererAPI* rendererAPI = nullptr;

		inline static Rendering::Shader* rectShader = nullptr;
		inline static Rendering::Shader* postProcessShader = nullptr;
		inline static Rendering::Shader* textShader = nullptr;

		inline static Rendering::FrameBuffer* currentFrameBuffer = nullptr;
		inline static Rendering::FrameBuffer* defaultFrameBuffer = nullptr;

		inline static Mesh* rectMesh = nullptr;

		inline static Rendering::VertexBuffer* fbVerts = nullptr;
		inline static Rendering::IndexBuffer* fbIndexes = nullptr;
		inline static Rendering::VertexArray* fbArray = nullptr;

		inline static unsigned int width = 0;
		inline static unsigned int height = 0;

		static glm::mat4 GenTransform( Vector2D position, Vector2D size, float rotation, Vector2D rotationOrigin );
	};
}
