#include "Console.h"

#include <imgui.h>

#include "CatWare/Log.h"
#include "CatWare/Graphics/Renderer/Renderer.h"

#include "Commands.h"

namespace CatWare
{
	ConVar::ConVar( const ConVarType& type, void* valuePtr )
	{
		this->type = type;
		this->valuePtr = valuePtr;
	}

	void Console::Init( )
	{
		AddCommand( new Commands::Exit );
		AddCommand( new Commands::Set );

		RegisterConVar( "pp_sharpness", ConVar( ConVarType::FLOAT, &Renderer::postProcess.sharpness ) );
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
				{
					consoleCommand->Run( splitString );
					return;
				}
			}
		}

		CW_ENGINE_LOG->Error( "Command %s not found", splitString[0].c_str( ) );
	}

	void Console::Draw( )
	{
		ImGui::Begin( "Console" );

		if ( ImGui::InputText( "Command", commandBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			RunString( std::string( commandBuffer ) );
			strcpy( commandBuffer, "" );
		}

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
