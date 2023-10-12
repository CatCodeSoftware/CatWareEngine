#include "Log.h"

#include <stdio.h>
#include <Windows.h>

// Should probably replace my impementation with a library like spdlog in the future - PT

namespace CatWare
{
	namespace Logging
	{
		static Logger* engineLogger;
		static Logger* gameLogger;

		// LOG CLASS
		Logger::Logger( std::string category, std::string logFile )
		{
			this->category = category;
			this->logFileLocation = logFileLocation;

			// for now no logging to file
		}

		Logger::~Logger( )
		{

		}


		void Logger::Error ( std::string text, ... )
		{
			text = "[ " + category + " ] " + text;

			va_list args;
			va_start ( args, text );

			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0xC0 );
			vprintf ( text.c_str( ), args );
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0F );
		}

		void Logger::Info ( std::string text, ... )
		{
			text = "[ " + category + " ] " + text;

			va_list args;
			va_start ( args, text );

			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0F );
			vprintf ( text.c_str( ), args );
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0F );

		}

		void Logger::Warning ( std::string text, ... )
		{
			text = "[ " + category + " ] " + text;

			va_list args;
			va_start ( args, text );

			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0xE0 );
			vprintf ( text.c_str( ), args );
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0F );
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