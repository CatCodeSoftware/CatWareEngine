#include "DebugUI.h"

#include <imgui.h>

#include "CatWare/Graphics/Renderer/Renderer.h"

namespace CatWare
{
	void DebugUI::Draw( )
	{
		if ( enabled )
		{
			ImGui::Begin( "Debug tools", &enabled );

			if ( ImGui::Button( "Post process UI" ) ) postProcessUIEnabled = true;
			if ( ImGui::Button( "Console" ) ) consoleEnabled = true;

			ImGui::End( );

			if ( consoleEnabled )
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

			if ( postProcessUIEnabled )
			{
				ImGui::Begin( "Post process", &postProcessUIEnabled );

				if ( ImGui::Button( "Reset" ) ) Renderer::postProcess = PostProcess( );

				ImGui::SliderFloat( "Brightnes", &Renderer::postProcess.brightness, -1, 1 );
				ImGui::SliderFloat( "Contrast", &Renderer::postProcess.contrast, 0, 10 );
				ImGui::SliderFloat( "Exposure", &Renderer::postProcess.exposure, -10, 10 );
				ImGui::SliderFloat( "Saturation", &Renderer::postProcess.saturation, -1, 1 );
				ImGui::SliderFloat( "Sharpness", &Renderer::postProcess.sharpness, -10, 10 );

				float colors[4] = {
					float( Renderer::postProcess.tint.r ) / 255.0f,
					float( Renderer::postProcess.tint.g ) / 255.0f,
					float( Renderer::postProcess.tint.b ) / 255.0f,
					float( Renderer::postProcess.tint.a ) / 255.0f,
				};

				ImGui::ColorPicker4( "Tint", colors );

				Renderer::postProcess.tint.r = colors[0] * 255;
				Renderer::postProcess.tint.g = colors[1] * 255;
				Renderer::postProcess.tint.b = colors[2] * 255;
				Renderer::postProcess.tint.a = colors[3] * 255;

				ImGui::End( );
			}
		}
	}

	void DebugUI::PrintToConsole( std::string text )
	{
		consoleContents.push_back( text );
	}

}
