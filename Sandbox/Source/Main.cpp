#include <CatWare.h>

#include <iostream>

#include <Windows.h>

using namespace CatWare;

extern "C"
{
	__declspec( dllexport ) unsigned long NvOptimusEnablement = 0x00000001;
}

class Sandbox : public CatWare::Application
{
public:
	Sandbox( ) {}
	~Sandbox( ) {}

	void PostInit( ) override
	{
		CW_LOG->Warning( "Hello :)" );
	}
};

CW_REGISTER_APP( Sandbox )