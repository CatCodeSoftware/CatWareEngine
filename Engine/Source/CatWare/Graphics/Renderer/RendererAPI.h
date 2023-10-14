#pragma once

#include "CatWare/Utils/Color.h"
#include "Buffer.h"

namespace CatWare
{
	namespace Rendering
	{
		class RendererAPI
		{
		public:
			enum class API
			{
				NONE = 0,
				OPENGL = 1
			};

			virtual void SetClearColor( Color clearColor ) = 0;
			virtual void Clear( ) = 0;

			virtual void SetViewportSize( int width, int height ) = 0;

			virtual void DrawIndexed( VertexArray* vertexArray ) = 0;
			
			virtual API GetRenderAPI( ) = 0;
		};
	}
}