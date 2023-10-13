#pragma once

#include "Core.h"

#include "Graphics/Window.h"

namespace CatWare
{
	class CATWARE_API Application
	{
	public:
		Application( );
		virtual ~Application( );

		virtual void Run( ) final;

		virtual void PostInit( ) {}
		virtual void Update( ) {}
		virtual void Draw( ) {}

	private:
		bool running = true;

		Window* window;
	};
}