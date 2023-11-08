/**
* @file EntryPoint.h
* 
* @breif Abstracts the main function, due to an oversight
* this header has to be included seperately from CatWare.h.
* 
* @author PointThink
*/

#pragma once

#include "Application.h"
#include "Core.h"

#define CW_REGISTER_APP(AppClass) CatWare::Application* CatWare::CreateApplication( ) { return new AppClass; }



namespace CatWare
{
	extern Application* CreateApplication( );
}

// TODO: Make the entry point use WinMain and windows subsystem in release/distrubution mode

#if !defined(CW_ENTRYPOINT_CREATED)
#	if defined(CW_PLATFORM_WIN64)
		int main( int argc, char** argv )
		{
			CatWare::CreateApplication( )->Run( );
		}
#	endif

#	define CW_ENTRYPOINT_CREATED

#endif