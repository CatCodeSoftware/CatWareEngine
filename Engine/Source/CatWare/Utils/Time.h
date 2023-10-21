#pragma once

#include <chrono>

namespace CatWare
{
	class Timer
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
}