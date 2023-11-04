#include <CatWare.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

Text::Font* font = nullptr;


class TestEntity : public Entity
{
public:
	Animation anim;

	TestEntity( )
	{
		className = "test";
		velocity = { 0, 0 };

		KeyFrame kf;
		kf.nextKeyFrameDelay = 1.0;

		AnimationElement element1;
		AnimationElement element2;

		element1.color = { 255, 255, 255, 255 };
		element2.color = { 0, 255, 0, 255 };

		element1.transform.position = { 0, 0 };
		element1.transform.rotation = 40;
		element1.transform.size = { 60, 60 };

		element1.textured = true;
		element1.textureID = "test_cat";

		element2.transform.position = { 50, 50 };
		element2.transform.rotation = 80;
		element2.transform.size = { 30, 30 };
		
		kf.elements.push_back( element1 );
		kf.elements.push_back( element2 );

		KeyFrame kf2;
		kf2.nextKeyFrameDelay = 1.0;

		AnimationElement element3;


		element3.color = { 255, 0, 255, 255 };

		element3.transform.position = { 0, 0 };
		element3.transform.rotation = 40;
		element3.transform.size = { 60, 60 };

		kf2.elements.push_back( element3 );

		anim.AddKeyFrame( kf );
		anim.AddKeyFrame( kf2 );
	}

	void Init( ) override
	{
		AttachPhysicsObject( 10, true, 100 );
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
				// Destroy( );
			}

			if ( Input::IsKeyPressed( Input::KEY_W ) )
			{
				object->force.y -= 250000;
			}
			if ( Input::IsKeyPressed( Input::KEY_S ) )
			{
				object->force.y += 250000;
			}
			if ( Input::IsKeyPressed( Input::KEY_A ) )
			{
				object->force.x -= 250000;
			}
			if ( Input::IsKeyPressed( Input::KEY_D ) )
			{
				object->force.x += 250000;
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
		//Renderer::DrawRectTextured( transform.position, transform.size, TextureManager::GetTexture( "test_cat" ), { 255, 255, 255, 255 } );
		Renderer::DrawRectTextured( transform.position, transform.size, Assets::textures.GetAsset( "test_cat" ), { 255, 255, 255, 255 }, transform.rotation );
		Renderer::renderOffset = Vector2D( 0, 0 ) - ( transform.position - Vector2D( 1600 / 2, 900 / 2 ) + transform.size / Vector2D( 2, 2 ) );

		//anim.Draw( transform.position, { 1, 1 }, transform.rotation );
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

		// Renderer::DrawString( "Hello world!", { 20, 20 }, 1, font, { 255, 255, 255, 255 } );
		// Renderer::DrawString( "Good to see you!", { font->GetStringSize( "Hello world!" ) + 40, 20 }, 1, font, { 255, 255, 255, 255 } );
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

		Assets::textures.Add( "test_cat", "cat.png" );
		Assets::sounds.Add( "meow", "meow.wav" );

		font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

		inGame = new InGame;
		SceneManager::SetScene( inGame );

		GlobalTime::frameRateLimited = true;
	}

	void PreDeInit( ) override
	{
		delete inGame;
		delete font;
	}
};

CW_REGISTER_APP( Sandbox )