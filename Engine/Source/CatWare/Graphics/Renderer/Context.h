#pragma once

namespace CatWare
{
	namespace Rendering
	{
		class RenderingContext
		{
		public:
			virtual void Init(  ) = 0;
			virtual void SwapBuffers( ) = 0;
		};
	}
}