#include "Console.h"

#include <imgui.h>

#include "CatWare/Log.h"

namespace CatWare
{
	Console::Console( )
	{
		AddCommand( new TestCommand );
	}


	void Console::RunString( std::string line )
	{
		bool inQuotes = false;

		std::string buffer;
		std::vector<std::string> splitString;

		// split the string
		for ( char c : line )
		{
			if ( c == '"' )
				inQuotes = !inQuotes;
			else if ( c == ' ' && c != '"' )
			{
				splitString.push_back( buffer );
				buffer = "";
			}
			else
				buffer += c;
		}

		splitString.push_back( buffer );

		// find the command with the first word
		for ( ConsoleCommand* consoleCommand : commands )
		{
			for ( std::string name : consoleCommand->GetNames( ) )
			{
				if ( name == splitString[0] )
					consoleCommand->Run( splitString );
			}
		}
	}

	void Console::Draw( )
	{
		ImGui::Begin( "Console" );

		if ( ImGui::InputText( "Command", commandBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue ) )
			RunString( std::string( commandBuffer ) );

		ImGui::BeginChild( "output" );

		for ( std::string text : consoleContents )
			ImGui::Text( text.c_str( ) );

		ImGui::EndChild( );
		ImGui::End( );
	}

	ConsoleCommand::ConsoleCommand( std::vector<std::string> names, unsigned requiredArgsCount )
	{
		this->names = names;
		this->requiredArgsCount = requiredArgsCount;
	}


	void TestCommand::Run( std::vector<std::string> args )
	{
		CW_ENGINE_LOG->Info( args[1] );
	}


}
