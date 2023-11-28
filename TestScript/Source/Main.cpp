#define EXPORT extern "C" __declspec(dllexport)

#include <CatWare.h>

using namespace CatWare;

Text::Font* font = nullptr;

int boxNumber = 0;

class TestEntityBehavior : public EntityBehavior
{
public:
	void Update( ) override
	{
		Transform* transform = GetComponent<Transform>( );

		if ( Input::IsKeyPressed( Input::KEY_W ) )
		{
			transform->position.y -= 300 * GlobalTime::GetDeltaTime( );
		}
		if ( Input::IsKeyPressed( Input::KEY_S ) )
		{
			transform->position.y += 300 * GlobalTime::GetDeltaTime( );
		}
		if ( Input::IsKeyPressed( Input::KEY_A ) )
		{
			transform->position.x -= 300 * GlobalTime::GetDeltaTime( );
		}
		if ( Input::IsKeyPressed( Input::KEY_D ) )
		{
			transform->position.x += 300 * GlobalTime::GetDeltaTime( );
		}
	}

	void Draw( )
	{
		
	}
};

void CreateTestEntity( EntityManager* manager, Vector2D position )
{
	manager->CreateEntity( "testEntity", { } )
		.AddComponent<Transform>( position, Vector2D( 128, 200 ) )
		.AddComponent<SpriteRenderer>( Color( 255, 255, 255, 255 ), "testTexture" )
		.AddComponent<EntityBehaviorComponent>( new TestEntityBehavior );
}

class InGame : public Scene
{
public:

	InGame( )
	{
		
	}

	void OnEnter( ) override
	{
		CreateTestEntity( &entityManager, { 128, 128 } );
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

	Assets::textures.Add( "testTexture", "gato.jpg" );
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