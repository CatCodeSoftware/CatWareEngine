#include <CatWare.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

Physics::PhysicsObject* object = nullptr;

class TestEntity : public Entity::Entity
{
public:
	TestEntity( )
	{
		className = "test";
		velocity = { 0, 0 };
	}

	void Init( ) override
	{
		object->transform = &transform;
		object->mass = 10;
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		Renderer::DrawRect( transform.position, transform.size, { 255, 0, 0, 255 } );
	}

	static Entity* Create( std::unordered_map<std::string, std::string> tags )
	{
		return new TestEntity;
	}
};

class InGame : public Scene
{
	Text::Font* font = nullptr;

public:
	InGame( )
	{
		object = new Physics::PhysicsObject;
		physicsWorld.AddObject( object );

		entityManager.CreateEntityByClassName( "test", { { 100, 100 }, { 64, 64 } }, { } );

		font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );

		physicsWorld.gravity = { 0, 1000 };
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );

		Renderer::DrawString( "Hello world!", { 20, 20 }, 1, font, { 255, 255, 255, 255 } );
	}
};

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
		Entity::EntityRegistry::RegisterEntity<TestEntity>( "test" );
		TextureManager::AddTexture( "test_cat", "cat.png" );

		SetScene( new InGame );
	}
};

CW_REGISTER_APP( Sandbox )