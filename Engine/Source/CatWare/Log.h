#pragma once

#include <string>

#include "CatWare/Core.h"

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
			~Logger( );

			void Warning( std::string text, ... );
			void Error( std::string text, ... );
			void Info( std::string text, ... );

		private:
			std::string category;
			std::string logFileLocation;
		};

		CATWARE_API void InitLoggers( );
		CATWARE_API void DeInitLoggers( );

		CATWARE_API Logger* GetEngineLogger( );
		CATWARE_API Logger* GetGameLogger( );
	}
}