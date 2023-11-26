#define EXPORT extern "C" __declspec(dllexport)

#include <CatWare.h>

using namespace CatWare;

Text::Font* font = nullptr;

int boxNumber = 0;

void CreateTestEntity( EntityManager* manager, Vector2D position )
{
	/*
	manager->CreateEntity( "testEntity", { } )
		.AddComponent<TransformComponent>( position, { 64, 64 } )
		.AddComponent<RectRenderer>( { 255, 255, 255, 255 } );
	*/
}

class InGame : public Scene
{
public:

	InGame( )
	{
		
	}

	void OnEnter( ) override
	{

	}

	void Update( ) override
	{

	}

	void Tick( ) override
	{
		if ( Input::IsKeyPressed( Input::KEY_UP ) )
		{
			GlobalTime::maxFPS++;
		}
		if ( Input::IsKeyPressed( Input::KEY_DOWN ) )
		{
			GlobalTime::maxFPS--;
		}

		if ( Input::IsMousePressed( 1 ) )
		{
		}
	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );
	}

	void DrawGUI( ) override
	{
		Renderer::DrawString( "FPS: " + std::to_string( 1.0 / GlobalTime::GetDeltaTime( ) ), { 20, 20 }, 1, font, { 255, 255, 255, 255 } );
	}
};

InGame* inGame;

EXPORT void PreInit( CatWare::InitConfig* config )
{
	config->windowWidth = 1600;
	config->windowHeight = 900;

	GlobalTime::modifier = 1.0;
}

EXPORT void PostInit( )
{
	font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

	inGame = new InGame;
	SceneManager::SetScene( inGame );

	GlobalTime::frameRateLimited = false;
	GlobalTime::maxFPS = 240;

	inGame - new InGame;
	SceneManager::SetScene( inGame );
}

EXPORT void Activate( )
{
	CW_LOG->Warning( "Activated script" );
}

EXPORT void DeInit( )
{
	delete font;
}