#pragma once

#include <chrono>

#include "Core.h"
#include "Graphics/Window.h"
#include "Scene.h"

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

		void SetScene( Scene* scene );

	protected:
		bool running = true;

		Window* window = nullptr;

	private:
		Scene* currentScene = nullptr;
	};

	CATWARE_API double GetDeltaTime( );
}