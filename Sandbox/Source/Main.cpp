#include <CatWare.h>

#include <iostream>
#include <Windows.h>

#include <CatWare.h>

using namespace CatWare;
using namespace CatWare::Rendering;

extern "C"
{
	__declspec( dllexport ) unsigned long NvOptimusEnablement = 0x00000001;
}

class TestEntity : public Entity::Entity
{
public:
	TestEntity( )
	{
		className = "test";
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

class Sandbox : public CatWare::Application
{
public:
	Sandbox( ) {}
	~Sandbox( ) {}

	Entity::EntityManager entityManager;

	void PostInit( ) override
	{
		Entity::EntityRegistry::RegisterEntity<TestEntity>( "test" );

		entityManager.CreateEntityByClassName( "test", { { 0, 0 }, { 90, 90 } }, { } );


		TextureManager::AddTexture( "test_cat", "cat.png" );

		Input::LoadBindsFromConfig( "binds.cfg" );
	}

	void Update( ) override
	{
		entityManager.Update( );

		if ( Input::GetBinding( "do_stuff" )->IsDown( ) )
		{
			CW_LOG->Info( "Did stuff" );
		}
	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );

		entityManager.Draw( );
	}
};

CW_REGISTER_APP( Sandbox )