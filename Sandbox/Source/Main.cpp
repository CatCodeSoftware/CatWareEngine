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

class Sandbox : public CatWare::Application
{
public:
	Sandbox( ) {}
	~Sandbox( ) {}

	Texture2D* texture;

	void PostInit( ) override
	{
		CW_LOG->Warning( "Hello :)" );

		// Load a texture
		texture = Texture2D::Create( "cat.png" );
	}

	void Draw( ) override
	{
		Renderer::Clear( { 40, 40, 40, 255 } );

		Renderer::DrawRectTextured( { 20, 20 }, { 60, 60 }, texture, { 255, 0, 255, 255 } );
	}
};

CW_REGISTER_APP( Sandbox )