#pragma once

#include <chrono>

#include "..\Core.h"

namespace CatWare
{
	class InternalTimer
	{
	public:
		double GetTime( );
		bool HasTimeElapsed( float time );

		/* this name might be confusing
		this just gets how many times the time has elapsed */
		int TimesTimeElapsed( float time );

		void Reset( );

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};

	class CATWARE_API GlobalTime
	{
	public:
		static double modifier;

		inline static double GetDeltaTime( )
		{
			if ( isTimePaused )
				return 0;

			return deltaTime * modifier;
		}

		inline static double GetTime( )
		{
			return time;
		}

		static void TimePaused( bool isPaused );
		static void SetDeltaTime( double deltaTime );

	private:
		static double time;

		static double deltaTime;

		static bool isTimePaused;
	};
}