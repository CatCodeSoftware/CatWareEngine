#pragma once

#include <imgui.h>
#include <string>

#include "CatWare/Core.h"
#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Time.h"
#include "CatWare/Types/Vector.h"
#include "Console.h"

namespace CatWare
{
	static inline float Lerp2( float v0, float v1, float t )
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


	enum class NotifyType
	{
		ERROR, WARNING
	};

	class Notification
	{
	public:
		Notification( std::string message, std::string category, NotifyType type )
		{
			this->message = message;
			this->category = category;
			this->type = type;

			fadeInStart = Time::GetTime( );
			fadeInEnd = fadeInStart + 1;

			fadeOutStart = fadeInEnd + 5;
			fadeOutEnd = fadeOutStart + 1;
		}

		std::string message;
		std::string category;
		NotifyType type;

		bool fadingIn, fadingOut;

		double fadeInStart;
		double fadeInEnd;

		double fadeOutStart;
		double fadeOutEnd;
	};

	class AnimatedWindow
	{
	public:
		Vector2D beginPos, endPos, size;

		double beginTime;
		double endTime;
		bool reversed;

		AnimatedWindow( Vector2D beginPos, Vector2D endPos, Vector2D size )
		{
			this->beginPos = beginPos;
			this->endPos = endPos;
			this->size = size;
		}

		AnimatedWindow( )
		{

		}

		void Start( bool reversed, double time )
		{
			this->reversed = reversed;
			beginTime = Time::GetTime( );
			endTime = beginTime + time;
		}

		void Animate( )
		{
			float timeProgress = ( Time::GetTime( ) - beginTime ) / ( endTime - beginTime );

			if ( timeProgress > 1 )
				timeProgress = 1;

			if ( reversed )
				timeProgress = 1 - timeProgress;

			// lerp it
			Vector2D position = { 0, 0 };
			position.x = Lerp2( beginPos.x, endPos.x, EaseOutCubic( 1 - timeProgress ) );
			position.y = Lerp2( beginPos.y, endPos.y, EaseOutCubic( 1 - timeProgress ) );

			ImGui::SetNextWindowPos( { float( position.x ), float( position.y ) } );
			// ImGui::SetNextWindowPos( { float( endPos.x ), float( endPos.y ) } );
			ImGui::SetNextWindowSize( { float( size.x ), float( size.y ) } );
		}
	};

	class CATWARE_API DebugUI
	{
	public:

		inline static bool consoleEnabled = false;
		inline static bool postProcessUIEnabled = false;

		static void Draw( );

		static void NotifyError( std::string text, std::string category );
		static void NotifyWarning( std::string text, std::string category );
		static void ShowErrorNotifications( );
		static void ShowEntityViewer( );

		static void Open();

		static inline bool IsOpen( ) { return enabled; }

	private:
		inline static std::vector<Notification> notifications;

		inline static bool enabled = false;

		inline static int debugToolsTab = 0;

		inline static AnimatedWindow console;
		inline static AnimatedWindow debugTools;
		inline static AnimatedWindow gameViewport;

		inline static Vector2D notificationsOrigin = { 20, 20 };
		inline static float notificationsSpacing = 20;
	};
}
