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

			virtual void Clear( Color clearColor ) = 0;
			
			virtual void DrawIndexed( VertexArray* vertexArray );
			
			static API GetRenderAPI( )
			{
				return API::OPENGL;
			}

		private:
			
		};
	}
}