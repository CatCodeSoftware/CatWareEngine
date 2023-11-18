#include <CatWare.h>
#include <CatWare/EntryPoint.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

Text::Font* font = nullptr;

class Box : public Entity
{
public:
	Color color = { 255, 255, 255, 255 };

	void Init( )
	{
		PolygonShape* boxShape = new PolygonShape;
		boxShape->SetAsRect( transform.size );

		AttachPhysicsObject( boxShape, true, 1, 0.9 );

		delete boxShape;

		color = { UInt8( Random::GetUInt( 0, 255 ) ), UInt8( Random::GetUInt( 0, 255 ) ), UInt8( Random::GetUInt( 0, 255 ) ), 255 };
	}

	void Draw( )
	{
		Renderer::DrawRect( transform.position - ( transform.size / Vector2D( 2, 2 ) ), transform.size, color, transform.rotation );
	}

	static Entity* Create( std::unordered_map<std::string, std::string> tags )
	{
		return new Box;
	}

	void Tick( )
	{
		if ( Input::IsKeyPressed( Input::KEY_SPACE ) )
		{
			GetAttachedPhysicsObject( )->ApplyImpulse( { 200000, 0 }, GetAttachedPhysicsObject( )->GetWorldCenter( ) );
		}
	}

	void Update( )
	{
		if ( transform.position.y > 900 )
		{
			if ( GetAttachedPhysicsObject( ) == nullptr )
				DetachPhysicsObject( );
		}
	}
};

class InGame : public Scene
{
public:
	AudioHandle* handle;

	Transform floorTransform = { { 800, 900 }, { 1600, 1 } };

	InGame( )
	{
		EntityRegistry::RegisterEntity<Box>( "box" );
	}

	void OnEnter( ) override
	{
		physicsWorld.SetGravity( { 0, 200 } );

		PolygonShape* floorShape = new PolygonShape;
		floorShape->SetAsRect( { 1600, 1 } );

		physicsWorld.CreateObject( &floorTransform, floorShape, false, 1, 0.3 );
		
		delete floorShape;
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
			entityManager.CreateEntityByClassName( "box", { Input::GetMouseMotion( ), { 64, 64 } }, { } );
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

class Sandbox : public CatWare::Application
{
public:
	Sandbox( )
	{
		initConfig.windowWidth = 1600;
		initConfig.windowHeight = 900;

		GlobalTime::modifier = 1.0;
	}

	~Sandbox( ) {}

	void PostInit( ) override
	{
		font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

		inGame = new InGame;
		SceneManager::SetScene( inGame );

		GlobalTime::frameRateLimited = false;
		GlobalTime::maxFPS = 240;

		Assets::sounds.Add( "a", "a.wav" );
	}

	void PreDeInit( ) override
	{
		delete inGame;
		delete font;
	}
};

CW_REGISTER_APP( Sandbox )