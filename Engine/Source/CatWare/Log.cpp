#include "Log.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef CW_PLATFORM_WIN64
#	include <Windows.h>
#endif

// Todo: Make this not suck
namespace CatWare
{
	namespace Logging
	{
		static Logger* engineLogger;
		static Logger* gameLogger;

		void Logger::SetColor(Color color)
		{
			#ifdef CW_PLATFORM_WIN64

			if ( color == Color::RED )
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xC0 );
			else if ( color == Color::YELLOW )
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xE0 );
			else
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0F );

			#else

			if ( color == Color::RED )
				std::cout << "\033[30;41m";
			else if ( color == Color::YELLOW )
				std::cout << "\033[30;43m";
			else
				std::cout << "\033[39;49m";

			#endif

			if ( color == Color::RED )
				Console::SetColor( ConsoleColor::RED );
			if ( color == Color::YELLOW )
				Console::SetColor( ConsoleColor::ORANGE );
			if ( color == Color::RESET )
				Console::SetColor( ConsoleColor::WHITE );
		}

		// LOG CLASS
		Logger::Logger( std::string category, std::string logPath )
		{
			this->category = category;
			this->logFileLocation = logFileLocation;

			logFile.open( logPath );
		}

		Logger::~Logger( )
		{
			logFile.close( );
		}


		// Log
		void InitLoggers( )
		{
			engineLogger = new Logger( "Engine", "engine.log" );
			gameLogger = new Logger( "Game", "game.log" );
		}

		void DeInitLoggers( )
		{
			delete engineLogger;
			delete gameLogger;
		}


		Logger* GetEngineLogger( ) { return engineLogger; }
		Logger* GetGameLogger( ) { return gameLogger; }
	}
}