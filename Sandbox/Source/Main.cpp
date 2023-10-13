#include <CatWare.h>

#include <iostream>

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