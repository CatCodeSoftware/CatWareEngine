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
			if ( script.script != nullptr )
				script.script->PreInit( &initConfig );
		}
	}

	void PostInit( )
	{
		for ( Script& script : scripts )
		{
			if ( script.script != nullptr )
				script.script->Start( );
		}

		for ( Script& script : scripts )
		{
			if ( script.script != nullptr )
				script.script->Activate( );
		}
	}

	void PreDeInit( )
	{
		for ( Script& script : scripts )
		{
			if ( script.script != nullptr )
				script.script->Exit( );
		}
	}
};

CW_REGISTER_APP( Runtime )