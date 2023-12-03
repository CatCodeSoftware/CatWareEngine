#include "Console.h"

#include <imgui.h>

namespace CatWare
{
	void Console::Draw( )
	{
		ImGui::Begin( "Console" );

		if ( ImGui::InputText( "Command", commandBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue ) )
			consoleContents.push_back( std::string( commandBuffer ) );

		ImGui::BeginChild( "output" );

		for ( std::string text : consoleContents )
			ImGui::Text( text.c_str( ) );

		ImGui::EndChild( );
		ImGui::End( );
	}


}