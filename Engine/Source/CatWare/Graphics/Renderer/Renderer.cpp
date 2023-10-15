#include "Renderer.h"

namespace CatWare
{
	namespace Rendering
	{
		RendererAPI* Renderer::rendererAPI = nullptr;

		void Renderer::Init( RendererAPI* a_rendererAPI )
		{
			rendererAPI = a_rendererAPI;
		}
	}
}