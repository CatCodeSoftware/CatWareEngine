#pragma once

#include <vector>
#include <string>

namespace CatWare
{
	class ConsoleCommand;

	class Console
	{
	public:
		inline void AddCommand( ConsoleCommand* command ) { commands.push_back( command ); }
		void RunString( std::string line );

		void Draw( );
		inline void Print( std::string text ) { consoleContents.push_back( text ); };

	private:
		inline static char commandBuffer[256];
		inline static std::vector<std::string> consoleContents;

		std::vector<ConsoleCommand*> commands;
	};

	class ConsoleCommand
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
}