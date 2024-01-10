#pragma once

#include <string>
#include <fstream>

#include "CatWare/Core.h"
#include "CatWare/Debug/DebugUI.h"

// some macros for easier access
#define CW_ENGINE_LOG CatWare::Logging::GetEngineLogger( )
#define CW_LOG CatWare::Logging::GetGameLogger( )

namespace CatWare
{
	namespace Logging
	{
		class CATWARE_API Logger
		{
		public:
			Logger( std::string category, std::string logFile );
			~Logger();

			template<typename... Args>
			void PrintGeneric( std::string text, Args... args )
			{
				text = "[" + category + "]" + text + "\n";

				std::string buffer = Format( text, args... );

				printf( buffer.c_str(  ) );
				Console::Print( std::string( buffer ) );
				logFile << buffer;
				logFile.flush( );
			}

			template<typename... Args>
			std::string Format( std::string text, Args... args )
			{
				size_t bufferSize = snprintf( nullptr, 0, text.c_str( ), args... );
				char* buffer = new char[bufferSize + 1];
				snprintf( buffer, bufferSize + 1, text.c_str( ), args... );

				std::string returnStr = std::string( buffer );
				delete[] buffer;

				return returnStr;
			}

			template<typename... Args>
			void Warning( std::string text, Args... args )
			{
				SetColor( Color::YELLOW );


				#ifdef CW_DEBUG
				DebugUI::NotifyWarning( Format( text, args... ), category );
				#endif

				text = "[Warning] " + text;
				PrintGeneric( text, args... );

				SetColor( Color::RESET );
			}

			template<typename... Args>
			void Error( std::string text, Args... args )
			{
				SetColor( Color::RED );

				DebugUI::NotifyError( Format( text, args... ), category );

				text = "[Error] " + text;
				PrintGeneric( text, args... );

				SetColor( Color::RESET );
			}

			template<typename... Args>
			void Info( std::string text, Args... args )
			{
				SetColor( Color::RESET );

				text = "[Info] " + text;
				PrintGeneric( text, args... );
			}

		private:
			std::string category;
			std::string logFileLocation;

			std::ofstream logFile;

			enum class Color
			{
				RED,
				YELLOW,
				RESET
			};

			void SetColor(Color color);
		};

		CATWARE_API void InitLoggers( );
		CATWARE_API void DeInitLoggers( );

		CATWARE_API Logger* GetEngineLogger( );
		CATWARE_API Logger* GetGameLogger( );
	}
}