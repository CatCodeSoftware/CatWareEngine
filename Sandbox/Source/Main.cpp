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
		
		AnimationElement element3;
		element3.transform.position = { 0, 0 };
		element3.transform.size = { 1, 1 };
		element3.color = { 255, 0, 0, 255 };

		kf.elements.push_back( element1 );
		kf.elements.push_back( element2 );
		kf.elements.push_back( element3 );

		anim.AddKeyFrame( kf );
	}

	void Init( ) override
	{
		AttachPhysicsObject( 10, true, 0.7 );
	}

	void Tick( ) override
	{
		Physics::PhysicsObject* object = GetAttachedPhysicsObject( );

		if ( object != nullptr )
		{
			if ( Input::IsKeyPressed( Input::KEY_SPACE ) )
			{
				DetachPhysicsObject( );
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
	}

	void Draw( ) override
	{
		//Renderer::DrawRectTextured( transform.position, transform.size, TextureManager::GetTexture( "test_cat" ), { 255, 255, 255, 255 } );
		Renderer::DrawRectTextured( transform.position, transform.size, TextureManager::GetTexture( "test_cat" ), { 255, 255, 255, 255 }, transform.rotation );

		// anim.Draw( transform.position, { 1, 1 }, transform.rotation );
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
		physicsWorld.gravity = { 0, 0 };
		Renderer::renderOffset = { 0, 0 };
	}

	void OnEnter( ) override
	{
		entityManager.CreateEntityByClassName( "test", { { 100, 100 }, { 64, 64 } }, { } );
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );

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
		TextureManager::AddTexture( "test_cat", "cat.png" );

		font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

		inGame = new InGame;
		SceneManager::SetScene( inGame );
	}

	void PreDeInit( ) override
	{
		delete inGame;
		delete font;
	}
};

CW_REGISTER_APP( Sandbox )