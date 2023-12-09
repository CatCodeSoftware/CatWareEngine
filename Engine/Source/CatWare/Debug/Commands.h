#pragma once

#include "Console.h"

namespace CatWare
{
	namespace Commands
	{
		class Exit : public ConsoleCommand
		{
		public:
			Exit( );
			void Run(std::vector<std::string> args) override;
		};

		class Set : public ConsoleCommand
		{
		public:
			Set( );
			void Run(std::vector<std::string> args) override;
		};

		class Map : public ConsoleCommand
		{
		public:
			Map( );
			void Run(std::vector<std::string> args) override;
		};
	}
}