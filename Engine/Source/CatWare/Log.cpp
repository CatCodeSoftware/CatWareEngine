#include "Log.h"

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

		// Some utils
		enum class Color
		{
			RED,
			YELLOW,
			RESET
		};

		void SetColor(Color color)
		{
			#ifdef CW_PLATFORM_WIN64

			if (color == RED )
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xC0 );
			else if ( color == YELLOW )
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xE0 );
			else
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0F );

			#else

			#endif
		}

		// LOG CLASS
		Logger::Logger( std::string category, std::string logFile )
		{
			this->category = category;
			this->logFileLocation = logFileLocation;

			// for now no logging to file
		}

		void Logger::Error( std::string text, ... )
		{
			text = "[ " + category + " ] " + text + "\n";

			va_list args;
			va_start( args, text );

			SetColor( Color::RED );
			vprintf( text.c_str( ), args );
			SetColor( Color::RESET );
		}

		void Logger::Info( std::string text, ... )
		{
			text = "[ " + category + " ] " + text + "\n";

			va_list args;
			va_start( args, text );

			SetColor( Color::RESET );
			vprintf( text.c_str( ), args );
		}

		void Logger::Warning( std::string text, ... )
		{
			text = "[ " + category + " ] " + text + "\n";

			va_list args;
			va_start( args, text );

			SetColor( Color::YELLOW );
			vprintf( text.c_str( ), args );
			SetColor( Color::RESET );
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