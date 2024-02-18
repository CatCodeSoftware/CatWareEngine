#include <CatWare.h>

#include "CatWare/UI/Widgets.h"

using namespace CatWare;

int i = 1;

Text::Font* font = nullptr;

class TestEntity : public Entity
{
	ParticleEmmiter emmiter;

public:
	DynamicBody* pBody;

	TextureRef texture = TextureRef( "gato.jpg" );

	TestEntity( ) { className = "testEntity"; }

	void Init( ) override
	{
		pBody = new DynamicBody( 10, 1, new AABBCollider( transform.position, transform.size ) );
		pBody->position = transform.position;
		SceneManager::GetScene( )->world.physicsWorld.AddBody( pBody );

		emmiter.gravity = { 0, 0 };
		emmiter.angle = -90;
		emmiter.spread = 360;
		emmiter.numParticles = 50;
		emmiter.numParticlesRandomness = 1;
		emmiter.delay = 0;
		emmiter.speed = 200;
		emmiter.speedRandomness = 400;
		emmiter.airResistance = 0.1;
		emmiter.lifetime = 20;
		emmiter.lifetimeRandomness = 10;
		emmiter.size = { 500, 500 };
		emmiter.endSize = { 1000, 1000 };

		emmiter.startColor = { 128, 128, 128, 10 };
		emmiter.endColor = { 255, 255, 255, 0 };
		emmiter.velocityStrech = false;

		emmiter.textured = true;
		emmiter.textureIDs = { "whitePuff00.png", "whitePuff01.png", "whitePuff02.png" };

		emmiter.once = true;
		// emmiter.colorVariation = { 10, 10, 0, 20 };
	}

	void Update( ) override
	{
		emmiter.position = transform.position + transform.size / 2;

		if ( Input::IsKeyPressed( Input::KEY_W ) )
			pBody->force.y -= 30000;
		if ( Input::IsKeyPressed( Input::KEY_S ) )
			pBody->force.y += 30000;
		if ( Input::IsKeyPressed( Input::KEY_A ) )
			pBody->force.x -= 30000;
		if ( Input::IsKeyPressed( Input::KEY_D ) )
			pBody->force.x += 30000;

		if ( pBody->position.y > 900 - transform.size.y )
		{
			pBody->position.y = 900 - transform.size.y;
			pBody->velocity.y = 0;
			pBody->force.y = 0;
		}

		transform.position = pBody->position + pBody->velocity * Time::GetDeltaTime( ); // smooth out the movement
		// Renderer::camera2D->SetFocus( transform.position + transform.size / 2 );

		emmiter.Emit( );
	}

	void Draw( ) { Renderer::DrawRectTextured( transform, texture.Get( ) ); }

	CW_ENTITY_CREATE( tags ) { return new TestEntity( ); }
};

void OnPress( ) { CW_LOG->Error( "Pressed a button" ); }

class InGame : public Scene
{
public:
	UI::Button button = UI::Button( "Hello world", &OnPress );

	InGame( )
	{
		world.physicsWorld.gravity = 400;

		button.position = { 20, 20 };
		button.size = { 300, 100 };
	}

	void OnEnter( ) override { world.LoadFromMapFile( "testMap.yaml" ); }

	void Update( ) override {}

	void Tick( ) override
	{
		if ( Input::IsKeyPressed( Input::KEY_UP ) )
		{
			CW_LOG->Warning( "ahh %d", i );
			i++;

			Time::maxFPS++;
		}
		if ( Input::IsKeyPressed( Input::KEY_DOWN ) )
		{
			Time::maxFPS--;

			CW_LOG->Error( "AHH %d", i );
			i++;
		}
	}

	void Draw( ) override { Renderer::Clear( { 40, 40, 40, 255 } ); }

	void DrawGUI( ) override
	{
		Renderer::DrawString(
			"FPS: " + std::to_string( 1.0 / Time::GetDeltaTime( ) ), { 22, 22 }, 1, font, { 0, 0, 0, 255 } );
		Renderer::DrawString(
			"FPS: " + std::to_string( 1.0 / Time::GetDeltaTime( ) ), { 20, 20 }, 1, font, { 255, 255, 255, 255 } );

		button.Draw( );
		button.Update( );
	}
};

InGame* inGame;

class Game : Script
{
public:
	void PreInit( CatWare::InitConfig* initConfig ) override
	{
		initConfig->windowWidth = 1920;
		initConfig->windowHeight = 1080;

		initConfig->windowFullscreen = true;

		Time::modifier = 1.0;
	}

	void Start( ) override
	{
		font = new Text::Font( "EngineRes/Fonts/Jura-Regular.ttf", 30 );
		UI::Style::font = font;

		Time::frameRateLimited = false;
		Time::maxFPS = 240;

		EntityRegistry::RegisterEntity< TestEntity >( "testEntity" );

		TextureManager::AddTexture( "gato.jpg", Rendering::TextureFilter::LINEAR );
		TextureManager::AddTexture( "whitePuff00.png", Rendering::TextureFilter::LINEAR );
		TextureManager::AddTexture( "whitePuff01.png", Rendering::TextureFilter::LINEAR );
		TextureManager::AddTexture( "whitePuff02.png", Rendering::TextureFilter::LINEAR );

		inGame = new InGame;
		SceneManager::SetScene( inGame );
	}

	void Activate( ) override {}

	void Exit( ) override { delete font; }
};

CW_REGISTER_SCRIPT( Game );
