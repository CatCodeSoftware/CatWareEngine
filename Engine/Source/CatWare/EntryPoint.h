#pragma once

#include "Application.h"
#include "Core.h"

#define CW_REGISTER_APP(AppClass) CatWare::Application* CatWare::CreateApplication( ) { return new AppClass; }

namespace CatWare
{
	extern Application* CreateApplication( );
}

#if defined(CW_PLATFORM_WIN64)
	int main( int argc, char** argv )
	{
		CatWare::CreateApplication( )->Run( );
	}
#endif