#include <CatWare.h>
#include <CatWare/EntryPoint.h>

#include "Script.h"

class Runtime : public CatWare::Application
{
public:
	std::vector<Script> scripts;

	Runtime( )
	{
		CatWare::Logging::InitLoggers( );

		scripts = LoadScripts( "./Scripts/" );

		for ( Script& script : scripts )
		{
			if ( script.fptrPreInit != nullptr )
				script.fptrPreInit( &initConfig );
		}
	}

	void PostInit( )
	{
		for ( Script& script : scripts )
		{
			if ( script.fptrPostInit != nullptr )
				script.fptrPostInit( );
		}

		for ( Script& script : scripts )
		{
			if ( script.fptrActivate != nullptr )
				script.fptrActivate( ); 
		}
	}

	void PreDeInit( )
	{
		for ( Script& script : scripts )
		{
			if ( script.fptrDeInit != nullptr )
				script.fptrDeInit( );
		}
	}
};

CW_REGISTER_APP( Runtime )