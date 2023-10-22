#include <CatWare.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

class TestEntity : public Entity::Entity
{
	unsigned int drawsRemaining = 1000;

public:
	Physics::RectCollider* rect1 = nullptr;
	Physics::RectCollider* rect2 = nullptr;

	TestEntity( )
	{
		className = "test";
		velocity = { 0, 0 };
	}

	void Init( ) override
	{
		rect2 = new Physics::RectCollider( transform.position, transform.size );
		rect1 = new Physics::RectCollider( { transform.position.x + 32, transform.position.y + 32 }, transform.size );
	}

	void Update( ) override
	{
		rect1->position = Input::GetMouseMotion( );
	}

	void Draw( ) override
	{
		Physics::CollisionInfo ci = Physics::TestCollisionRectRect( rect1, rect2 );

		Renderer::DrawRect( rect1->position, rect1->size, { 255, 0, 0, 64 } );
		Renderer::DrawRect( rect2->position, rect2->size, { 0, 0, 255, 64 } );
		
		Renderer::DrawRect( ci.pointA, { 2, 2 }, { 0, 255, 0, 255 } );
		Renderer::DrawRect( ci.pointB, { 2, 2 }, { 255, 0, 0, 255 } );
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
		entityManager.CreateEntityByClassName( "test", { { 100, 100 }, { 64, 64 } }, { } );

		font = new Text::Font( "EngineRes/Fonts/Oxanium-Regular.ttf", 50 );
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );

		//Renderer::DrawString( "Hello world!", { 20, 20 }, 1, font, { 255, 255, 255, 255 } );
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