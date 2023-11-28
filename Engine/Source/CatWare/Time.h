#pragma once

#include <chrono>

#include "Core.h"

namespace CatWare
{
	class InternalTimer
	{
	public:
		double GetTime( );
		bool HasTimeElapsed( float time );

		/* this name might be confusing
		this just gets how many times the time has elapsed */
		int TimesTimeElapsed( double time );

		void Reset( );

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};

	class CATWARE_API Time
	{
	public:
		static double modifier;

		static float ticksPerSecond;
		static float maxFPS;
		static bool frameRateLimited;

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
		static bool isTimePaused;

		static double deltaTime;
	};
}