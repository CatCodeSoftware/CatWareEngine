#include "DebugUI.h"

#include <imgui.h>
#include <imgui_internal.h>

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

	void DebugUI::Draw( ){

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
			ImGui::SetNextWindowSize( { float( Renderer::GetScreenSize( ).x ), float( Renderer::GetScreenSize( ).y * 0.6 ) } );

			ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0 );

			Console::Draw( );

			ImGui::PopStyleVar( 1 );

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
