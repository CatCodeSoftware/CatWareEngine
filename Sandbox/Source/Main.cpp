#include <CatWare.h>

#include <iostream>

class Sandbox : public CatWare::Application
{
public:
	Sandbox( ) {}
	~Sandbox( ) {}

	void Run( )
	{
		CatWare::Logging::InitLoggers( );

		CW_LOG->Warning( "Running application" );
	}
};

CW_REGISTER_APP( Sandbox )