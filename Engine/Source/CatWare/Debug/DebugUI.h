#pragma once

#include <string>

#include "CatWare/Core.h"
#include "CatWare/Time.h"
#include "CatWare/Types/Vector.h"
#include "Console.h"

namespace CatWare
{
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

	class CATWARE_API DebugUI
	{
	public:

		inline static bool consoleEnabled = false;
		inline static bool postProcessUIEnabled = false;

		static void Draw( );

		static void NotifyError( std::string text, std::string category );
		static void NotifyWarning( std::string text, std::string category );
		static void ShowErrorNotifications( );

		static void Open();

	private:
		inline static std::vector<Notification> notifications;

		inline static bool enabled = false;

		inline static double consoleDropDownReverse = false;
		inline static double consoleDropDownTime = 0.5;
		inline static double consoleDropDownBegin = 0;
		inline static double consoleDropDownEnd = 0;

		inline static Vector2D notificationsOrigin = { 20, 20 };
		inline static float notificationsSpacing = 20;
	};
}
