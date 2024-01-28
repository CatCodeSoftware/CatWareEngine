#include "DebugUI.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "CatWare/GameState/Scene.h"
#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Time.h"


namespace CatWare
{
	void DebugUI::Open( )
	{
		double consoleSize = 0.3;
		double debugToolsSizeX = 0.3;
		double debugToolsSizeY = 0.7;

		console = AnimatedWindow( { 0, Renderer::GetScreenSize( ).y * -consoleSize }, { 0, 0 }, { Renderer::GetScreenSize( ).x, Renderer::GetScreenSize( ).y * consoleSize } );
		debugTools = AnimatedWindow( { -( Renderer::GetScreenSize( ).x * debugToolsSizeX ), Renderer::GetScreenSize( ).y - ( Renderer::GetScreenSize( ).y * debugToolsSizeY ) }, { 0, Renderer::GetScreenSize( ).y - ( Renderer::GetScreenSize( ).y * debugToolsSizeY ) }, { Renderer::GetScreenSize( ).x * debugToolsSizeX, Renderer::GetScreenSize( ).y * debugToolsSizeY } );
		gameViewport = AnimatedWindow( { Renderer::GetScreenSize( ).x + ( Renderer::GetScreenSize( ).x * debugToolsSizeX ), Renderer::GetScreenSize( ).y * consoleSize }, { Renderer::GetScreenSize( ).x * debugToolsSizeX, Renderer::GetScreenSize( ).y * consoleSize }, { Renderer::GetScreenSize( ).x * ( 1.0 - debugToolsSizeX ), Renderer::GetScreenSize( ).y * ( 1.0 - consoleSize ) } );

		console.Start( enabled, 0.5 );
		debugTools.Start( enabled, 0.5 );
		gameViewport.Start( enabled, 0.5 );
		enabled = !enabled;

		Console::scrollToBottom = true;
	}

	void DebugUI::Draw( )
	{

		if ( enabled || Time::GetTime( ) < console.endTime )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0 );

			console.Animate( );
			Console::Draw( );

			debugTools.Animate( );
			ImGui::Begin( "Debug tools", NULL, ImGuiWindowFlags_NoDecoration );

			ImGui::SameLine( );
			if ( ImGui::Button( "Entities" ) ) debugToolsTab = 0;
			ImGui::SameLine( );
			if ( ImGui::Button( "Time" ) ) debugToolsTab = 1;
			ImGui::SameLine( );
			if ( ImGui::Button( "Physics" ) ) debugToolsTab = 2;

			ImGui::BeginChild( "Tool window" );

			switch ( debugToolsTab )
			{
			case 0:
				ShowEntityViewer( );
				break;
			case 1:
				ImGui::Text( "Time: %lf", Time::GetTime(  ) );
				ImGui::SliderFloat( "Time multiplier", &Time::modifier, 0.25, 2 );

				if ( ImGui::Button( "Reset" ) )
					Time::modifier = 1;

				break;

			case 2:
				ImGui::Checkbox( "Draw colliders", &SceneManager::GetScene( )->world.physicsWorld.drawColliders );

				break;
			}

			ImGui::EndChild( );

			ImGui::End( );

			gameViewport.Animate( );
			ImGui::Begin( "Game view", NULL, ImGuiWindowFlags_NoDecoration );

			ImGui::Image( ( ImTextureID ) Renderer::GetRenderTarget( )->GetColorAttachment( )->GetTextureID( ), ImGui::GetContentRegionAvail( ), { 0, 1 }, { 1, 0 } );

			ImGui::End( );

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
				position.x = Lerp2( beginPos.x, desiredPos.x, EaseOutCubic( 1 - timeProgress ) );
				position.y = Lerp2( beginPos.y, desiredPos.y, EaseOutCubic( 1 - timeProgress ) );

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
				position.x = Lerp2( beginPos.x, desiredPos.x, EaseOutCubic( 1 - timeProgress ) );
				position.y = Lerp2( beginPos.y, desiredPos.y, EaseOutCubic( 1 - timeProgress ) );

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

	void DebugUI::ShowEntityViewer( )
	{
		for ( Entity* entity : SceneManager::GetScene( )->world.entities.entities )
		{
			if ( ImGui::TreeNode( ( entity->GetClassName( ) + " - " + std::to_string( entity->GetID( ) ) ).c_str(  ) ) )
			{
				ImGui::Indent( 16 );

				if ( ImGui::Button( "Destroy" ) ) entity->Destroy( );

				ImGui::InputDouble( "Position X", &entity->transform.position.x );
				ImGui::InputDouble( "Position Y", &entity->transform.position.y );
				ImGui::InputDouble( "Size X", &entity->transform.size.x );
				ImGui::InputDouble( "Size Y", &entity->transform.size.y );
				ImGui::InputFloat( "Rotation", &entity->transform.rotation );
				ImGui::InputInt( "Layer", &entity->transform.layer );

				ImGui::Unindent( 16 );

				ImGui::TreePop(  );
			}
		}
	}
} // namespace CatWare
