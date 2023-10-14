#include <CatWare.h>

#include <iostream>

#include <Windows.h>

extern "C" {
	_declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;
}

using namespace CatWare;

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