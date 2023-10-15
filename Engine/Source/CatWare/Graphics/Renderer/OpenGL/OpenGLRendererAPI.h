#pragma once

#include "CatWare/Graphics/Renderer/RendererAPI.h"

namespace CatWare
{
	namespace Rendering
	{
		namespace OpenGL
		{
			class OpenGLAPI : public RendererAPI
			{
				virtual void SetClearColor( Color clearColor ) override;
				virtual void Clear( ) override;

				virtual void SetBlendAlpha( bool blend ) override;

				virtual void SetViewportSize( int width, int height ) override;

				virtual void DrawIndexed( VertexArray* vertexArray ) override;

				virtual RendererAPI::API GetRenderAPI( ) override;
			};
		}
	}
}