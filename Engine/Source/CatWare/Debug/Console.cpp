#include "Console.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Log.h"

#include "Commands.h"

namespace CatWare
{
	ConVar::ConVar( )
	{
		this->isValid = false;
		this->type = ConVarType::BOOL;
		this->valuePtr = nullptr;
	}

	ConVar::ConVar( const ConVarType& type, void* valuePtr )
	{
		this->isValid = true;
		this->type = type;
		this->valuePtr = valuePtr;
	}

	void Console::Init( )
	{
		AddCommand( new Commands::Exit );
		AddCommand( new Commands::Set );
		AddCommand( new Commands::Map );
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
		ImGui::Begin( "Console", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration );

		if ( ImGui::InputText( "Command", commandBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			RunString( std::string( commandBuffer ) );
			strcpy( commandBuffer, "" );
		}

		// autocomplete
		// get all commands that begin with the contents of the buffer
		std::vector<std::string> autocompleteCommands;

		for ( ConsoleCommand* command : commands )
		{
			for ( std::string commandName : command->GetNames( ) )
			{
				bool beginsWithBuffer = true;

				for ( unsigned int i = 0; i < strlen( commandBuffer ); i++ )
				{
					if ( i > commandName.length( ) || commandName[i] != commandBuffer[i] )
						beginsWithBuffer = false;
				}

				if ( beginsWithBuffer )
					autocompleteCommands.push_back( commandName );
			}
		}

		if ( ImGui::BeginCombo( "##combo", NULL, ImGuiComboFlags_NoPreview ) )
		{
			for ( std::string autocompleteCommand : autocompleteCommands )
				if ( ImGui::Selectable( autocompleteCommand.c_str( ) ) )
					strcpy( commandBuffer, autocompleteCommand.c_str( ) );

			ImGui::EndCombo( );
		}

		ImGui::BeginChild( "output" );

		for ( ConsoleText text : consoleContents )
		{
			switch ( text.color )
			{
			case ConsoleColor::RED: ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1, 0, 0, 1 ) ); break;
			case ConsoleColor::ORANGE: ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1, 0.8, 0, 1 ) ); break;
			case ConsoleColor::WHITE: ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1, 1, 1, 1 ) ); break;
			}

			ImGui::Text( text.text.c_str( ) );

			ImGui::PopStyleColor( 1 );
		}

		ImGui::EndChild( );
		ImGui::End( );
	}

	ConsoleCommand::ConsoleCommand( std::vector<std::string> names, unsigned requiredArgsCount )
	{
		this->names = names;
		this->requiredArgsCount = requiredArgsCount;
	}

}
