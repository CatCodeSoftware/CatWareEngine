#include <CatWare.h>
#include <CatWare/EntryPoint.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

Text::Font* font = nullptr;

class TestEntity : public Entity
{
public:
	Physics::RectCollider* collider;

	Animation anim = Animation( "anim.txt" );

	TestEntity( )
	{
		className = "test";
		velocity = { 0, 0 };
	}

	void Init( ) override
	{
		collider = new Physics::RectCollider( transform.position, { 64, 64 } );

		AttachPhysicsObject( 10, true, 0.9 );
		Physics::PhysicsObject* po = GetAttachedPhysicsObject( );

		po->AttachCollider( collider );
		po->resistance = 0.5;
		po->collidable = true;
		po->movable = true;

		AudioEngine::audioListener = AudioListener2D( transform.position, 1 );
	}

	void Tick( ) override
	{
		Physics::PhysicsObject* object = GetAttachedPhysicsObject( );

		if ( object != nullptr )
		{
			if ( Input::IsKeyPressed( Input::KEY_SPACE ) )
			{
				DetachPhysicsObject( );
				Destroy( );
			}

			if ( Input::IsKeyPressed( Input::KEY_W ) )
			{
				object->force.y -= 25000;
			}
			if ( Input::IsKeyPressed( Input::KEY_S ) )
			{
				object->force.y += 25000;
			}
			if ( Input::IsKeyPressed( Input::KEY_A ) )
			{
				object->force.x -= 25000;
			}
			if ( Input::IsKeyPressed( Input::KEY_D ) )
			{
				object->force.x += 25000;
			}
		}

		if ( Input::IsKeyPressed( Input::KEY_LEFT ) )
		{
			transform.rotation -= 2;
		}

		if ( Input::IsKeyPressed( Input::KEY_RIGHT ) )
		{
			transform.rotation += 2;
		}

		AudioEngine::audioListener.position = transform.position;
	}

	void Draw( ) override
	{
		anim.Draw( transform.position, transform.size );
	}

	static Entity* Create( std::unordered_map<std::string, std::string> tags )
	{
		return new TestEntity;
	}
};

class InGame : public Scene
{
public:
	AudioHandle* handle;

	InGame( )
	{
		handle = AudioEngine::PlaySound2D( Assets::sounds.GetAsset( "meow" ), { 1280 / 2, 720 / 2 }, 1.0, 700 );

		handle->SetLooping( true );

		physicsWorld.gravity = { 0, 0 };
		Renderer::renderOffset = { 0, 0 };
	}

	void OnEnter( ) override
	{
		entityManager.CreateEntityByClassName( "test", { { 0, 0 }, { 64, 64 } }, { } );
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

		if ( Input::IsKeyPressed( Input::KEY_LEFT ) )
		{
			
		}

		if ( Input::IsKeyPressed( Input::KEY_RIGHT ) )
		{
			
		}
	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );

		Renderer::DrawString( "goober", { 90, 90 }, 1, font );
		Renderer::DrawRect( { 1280 / 2 - 2, 720 / 2 - 2 }, { 4, 4}, { 255, 0, 0, 255 } );
	}

	void DrawGUI( ) override
	{
		Renderer::DrawString( "FPS: " + std::to_string( 1.0 / GlobalTime::GetDeltaTime( ) ), { 20, 20 }, 1, font, { 255, 255, 255, 255 } );
	}
};

InGame* inGame;

class Sandbox : public CatWare::Application
{
public:
	Sandbox( )
	{
		initConfig.windowWidth = 1600;
		initConfig.windowHeight = 900;

		Renderer::renderOffset = { 0, 0 };

		GlobalTime::modifier = 1.0;
	}

	~Sandbox( ) {}

	void PostInit( ) override
	{
		EntityRegistry::RegisterEntity<TestEntity>( "test" );

		Assets::textures.Add( "testcat", "cat.png" );
		Assets::textures.Add( "circle", "circle.png" );

		Assets::sounds.Add( "meow", "meow.wav" );

		font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

		inGame = new InGame;
		SceneManager::SetScene( inGame );

		GlobalTime::frameRateLimited = true;
		GlobalTime::maxFPS = 240;
	}

	void PreDeInit( ) override
	{
		delete inGame;
		delete font;
	}
};

CW_REGISTER_APP( Sandbox )