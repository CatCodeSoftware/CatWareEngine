#include "DebugUI.h"

#include <imgui.h>

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Time.h"

static inline float Lerp( float v0, float v1, float t )
{
	return v0 + t * ( v1 - v0 );
}

static inline float EaseIn( float x )
{
	return x * x * x;
}

static inline float EaseOutCubic( float x )
{
	return 1 - pow( x, 3 );
}

namespace CatWare
{
	void DebugUI::Open( )
	{
		consoleDropDownBegin = Time::GetTime( );
		consoleDropDownEnd = consoleDropDownBegin + consoleDropDownTime;

		consoleDropDownReverse = enabled;
		enabled = !enabled;

		Console::scrollToBottom = true;
	}

	void DebugUI::Draw( )
	{
		if ( enabled || Time::GetTime( ) < consoleDropDownEnd )
		{
			// get progression of animation
			Vector2D beginPos = { 0, Renderer::GetScreenSize( ).y * -0.6 };
			Vector2D desiredPos = { 0, 0 };

			float timeProgress = ( Time::GetTime( ) - consoleDropDownBegin ) / consoleDropDownTime;

			if ( timeProgress > 1 )
				timeProgress = 1;

			if ( consoleDropDownReverse )
				timeProgress = 1 - timeProgress;

			// lerp it
			Vector2D position = { 0, 0 };
			position.y = EaseIn( 1 - timeProgress ) * Lerp( beginPos.y, desiredPos.y, timeProgress );

			ImGui::SetNextWindowPos( { float( position.x ), float( position.y ) } );
			ImGui::SetNextWindowSize(
				{ float( Renderer::GetScreenSize( ).x ), float( Renderer::GetScreenSize( ).y * 0.6 ) } );

			ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0 );

			Console::Draw( );

			ImGui::PopStyleVar( 1 );

			if ( postProcessUIEnabled )
			{
				ImGui::Begin( "Post process", &postProcessUIEnabled );

				if ( ImGui::Button( "Reset" ) )
					Renderer::postProcess = PostProcess( );

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
		} else
			ShowErrorNotifications( );
	}

	void DebugUI::NotifyError( std::string text, std::string category )
	{
		notifications.push_back( Notification( text, category, NotifyType::ERROR ) );
	}

	void DebugUI::NotifyWarning( std::string text, std::string category )
	{
		notifications.push_back( Notification( text, category, NotifyType::WARNING ) );
	}

	void DebugUI::ShowErrorNotifications( )
	{
		Vector2D currentPos = notificationsOrigin;

		for ( int index = 0; index < notifications.size( ); )
		{
			Notification notification = notifications[index];

			if ( Time::GetTime( ) < notification.fadeOutStart )
			{
				// get progression of animation
				Vector2D beginPos = { currentPos.x, -30 };
				Vector2D desiredPos = currentPos;

				float timeProgress = ( Time::GetTime( ) - notification.fadeInStart ) / ( notification.fadeInEnd -  notification.fadeInStart );

				if ( timeProgress > 1 )
					timeProgress = 1;

				// lerp it
				Vector2D position = notificationsOrigin;
				position.x = Lerp( beginPos.x, desiredPos.x, EaseOutCubic( 1 - timeProgress ) );
				position.y = Lerp( beginPos.y, desiredPos.y, EaseOutCubic( 1 - timeProgress ) );

				ImGui::SetNextWindowPos( { float( position.x ), float( position.y ) } );
			}
			else
			{
				// get progression of animation
				Vector2D beginPos = currentPos;
				Vector2D desiredPos = { -100, currentPos.y };

				float timeProgress = ( Time::GetTime( ) - notification.fadeOutStart ) / ( notification.fadeOutEnd -  notification.fadeOutStart );

				if ( timeProgress > 1 )
				{
					notifications.erase( notifications.begin( ) + index );
					continue;
				}

				// lerp it
				Vector2D position = notificationsOrigin;
				position.x = Lerp( beginPos.x, desiredPos.x, EaseOutCubic( 1 - timeProgress ) );
				position.y = Lerp( beginPos.y, desiredPos.y, EaseOutCubic( 1 - timeProgress ) );

				ImGui::SetNextWindowPos( { float( position.x ), float( position.y ) } );
			}

			ImGui::Begin( ( "notification" + std::to_string( index ) ).c_str(  ), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse );

			switch ( notification.type )
			{
			case NotifyType::ERROR: ImGui::TextColored( ImVec4( 1, 0, 0, 1 ),  "ERROR" ); break;
			case NotifyType::WARNING: ImGui::TextColored( ImVec4( 1, 1, 0, 1 ),  "WARNING" ); break;
			}

			currentPos.y += notificationsSpacing + ImGui::GetWindowHeight( );

			ImGui::Text( notification.message.c_str( ) );

			ImGui::End( );
			index++;
		}
	}

} // namespace CatWare
