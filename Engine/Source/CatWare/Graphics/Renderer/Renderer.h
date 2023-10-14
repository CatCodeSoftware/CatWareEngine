#pragma once

namespace CatWare
{
	namespace Rendering
	{
		enum class RenderApi
		{
			NONE = 0,
			OPENGL = 1
		};

		class Renderer
		{
		public:

			inline static RenderApi GetRenderApi( ) { return api; }

		private:
			static RenderApi api;
		};
	}
}