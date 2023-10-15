#pragma once

#include "CatWare/Core.h"
#include "RendererAPI.h"
#include "OpenGL/OpenGLRendererAPI.h"

namespace CatWare
{
	namespace Rendering
	{
		class Renderer
		{
		public:
			static void Init( RendererAPI* a_rendererAPI );

			static RendererAPI::API GetAPI( ) { return rendererAPI->GetRenderAPI( ); }

		private:
			static RendererAPI* rendererAPI;
		};
	}
}