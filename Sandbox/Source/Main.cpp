#include <CatWare.h>

#include <iostream>

using namespace CatWare;
using namespace CatWare::Rendering;

class TestEntity : public Entity::Entity
{
public:
	TestEntity( )
	{
		className = "test";
		// velocity = { 100, 100 };
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		Renderer::DrawRectTextured( transform.position, transform.size, TextureManager::GetTexture( "test_cat" ), { 255, 255, 255, 255 } );
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
		entityManager.CreateEntityByClassName( "test", { { 0, 0 }, { 180, 180 } }, { } );
	}

	void Update( ) override
	{

	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );
	}
};

class Sandbox : public CatWare::Application
{
public:
	Sandbox( )
	{
		initConfig.windowWidth = 1600;
		initConfig.windowHeight = 900;

		Renderer::renderOffset = { 800 - 60, 450 - 60 };
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