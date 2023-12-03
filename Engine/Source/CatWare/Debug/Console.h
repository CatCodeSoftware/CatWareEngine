#pragma once

#include <vector>
#include <string>

#include "CatWare/Core.h"

namespace CatWare
{
	class ConsoleCommand;

	class CATWARE_API Console
	{
	public:
		Console( );

		inline void AddCommand( ConsoleCommand* command ) { commands.push_back( command ); }
		void RunString( std::string line );

		void Draw( );
		inline void Print( std::string text ) { consoleContents.push_back( text ); };

	private:
		inline static char commandBuffer[256];
		inline static std::vector<std::string> consoleContents;

		std::vector<ConsoleCommand*> commands;
	};

	class CATWARE_API ConsoleCommand
	{
	public:
		ConsoleCommand( std::vector<std::string> names, unsigned int requiredArgsCount );

		virtual void Run( std::vector<std::string> args ) = 0;

		inline std::vector<std::string> GetNames( ) { return names; }
		inline unsigned int GetRequiredArgs( ) { return requiredArgsCount; }

	private:
		std::vector<std::string> names;
		unsigned int requiredArgsCount;
	};

	class CATWARE_API TestCommand : public ConsoleCommand
	{
	public:
		TestCommand( ) : ConsoleCommand( { "test" }, 0 )
		{

		}

		void Run( std::vector<std::string> args ) override;
	};
}