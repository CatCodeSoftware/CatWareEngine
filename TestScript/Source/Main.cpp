#ifdef CW_PLATFORM_WIN64
#	define EXPORT extern "C" __declspec(dllexport)
#else
#	define EXPORT extern "C"
#endif

#include <CatWare.h>
#include <CatWare/Physics/Physics.h>

using namespace CatWare;

Text::Font* font = nullptr;

PhysicsWorld physicsWorld;


class TestEntity : public Entity
{
public:
	DynamicBody* pBody;

	void Init( ) override
	{
		pBody = new DynamicBody( 10, new RectCollider( transform.position,  transform.size ) );
		pBody->position = transform.position;
		physicsWorld.AddBody( pBody );
	}

	void Update( ) override
	{
		if ( Input::IsKeyPressed( Input::KEY_W ) )
			pBody->force.y -= 30000;
		if ( Input::IsKeyPressed( Input::KEY_S ) )
			pBody->force.y += 30000;
		if ( Input::IsKeyPressed( Input::KEY_A ) )
			pBody->force.x -= 30000;
		if ( Input::IsKeyPressed( Input::KEY_D ) )
			pBody->force.x += 30000;

		transform.position = pBody->position + pBody->velocity * Time::GetDeltaTime( ); // smooth out the movements
	}

	void Draw( )
	{
		Renderer::DrawRectTextured( transform.position, transform.size, Assets::textures.GetAsset( "testTexture" ), { 255, 255, 255, 255 }, transform.rotation );
	}

	static Entity* Create( std::unordered_map<std::string, std::string> tags )
	{
		return new TestEntity;
	}
};

class InGame : public Scene
{
public:
	InGame( )
	{
		SurfaceBody* surface = new SurfaceBody( 0.9, new RectCollider( { 0, 0 }, { 1600, 900 } ) );
		physicsWorld.AddBody( surface );
	}

	void OnEnter( ) override
	{
		entityManager.CreateEntityByClassName( "testEntity", { { 64, 64 }, { 64, 64 } }, { } );
		// entityManager.CreateEntityByType<TestEntity>( { { 64, 64 }, { 64, 64 } }, { } );
	}

	void Update( ) override
	{

	}

	void Tick( ) override
	{
		physicsWorld.Step( 1.0f / Time::ticksPerSecond );

		if ( Input::IsKeyPressed( Input::KEY_UP ) )
		{
			Time::maxFPS++;
		}
		if ( Input::IsKeyPressed( Input::KEY_DOWN ) )
		{
			Time::maxFPS--;
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
		Renderer::DrawString( "FPS: " + std::to_string( 1.0 / Time::GetDeltaTime( ) ), { 20, 20 }, 1, font, { 255, 255, 255, 255 } );
	}
};

InGame* inGame;

EXPORT void PreInit( CatWare::InitConfig* config )
{
	config->windowWidth = 1600;
	config->windowHeight = 900;

	Time::modifier = 1.0;
}

EXPORT void PostInit( )
{
	font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

	Time::frameRateLimited = false;
	Time::maxFPS = 240;

	EntityRegistry::RegisterEntity<TestEntity>( "testEntity" );

	Assets::textures.Add( "testTexture", "gato.jpg" );

	inGame = new InGame;
	SceneManager::SetScene( inGame );
}

EXPORT void Activate( )
{

}

EXPORT void DeInit( )
{
	delete font;
}