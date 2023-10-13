#include "Core.h"

#include "CatWare/Utils/Log.h"

namespace CatWare
{
	void Init( )
	{
		CatWare::Logging::InitLoggers( );

		CW_ENGINE_LOG->Info( "Initializing engine" );
	}
}