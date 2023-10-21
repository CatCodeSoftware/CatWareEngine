#include <CatWare.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

class TestEntity : public Entity::Entity
{
	unsigned int drawsRemaining = 1000;

public:
	TestEntity( )
	{
		className = "test";
		velocity = { 100, 100 };
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		if ( drawsRemaining != 0 )
		{
			Renderer::DrawRectTextured( transform.position, transform.size, TextureManager::GetTexture( "test_cat" ), { 255, 255, 255, 255 } );
			drawsRemaining--;
		}
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
		entityManager.CreateEntityByClassName( "test", { { 0, 0 }, { 60, 60 } }, { } );

		font = new Text::Font( "C:/Windows/Fonts/arial.ttf", 50 );
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