#include "Time.h"

#include <cmath>

namespace CatWare
{
	double InternalTimer::GetTime( )
	{
		std::chrono::duration<float> fsec = std::chrono::high_resolution_clock::now( ) - startTime;
		return fsec.count( );
	}

	bool InternalTimer::HasTimeElapsed( float time )
	{
		if ( GetTime( ) >= time )
		{
			return true;
		}

		return false;
	}

	int InternalTimer::TimesTimeElapsed( double time )
	{
		double result = GetTime( ) / time;
		return GetTime( ) / time;
	}

	void InternalTimer::Reset( )
	{
		startTime = std::chrono::high_resolution_clock::now( );
	}

	
	double Time::time = 0;

	double Time::deltaTime = 0;
	double Time::modifier = 1.0;

	bool Time::isTimePaused = false;

	float Time::ticksPerSecond = 60; // 60 seems like a nice default value
	float Time::maxFPS = 240;
	bool Time::frameRateLimited = true;

	void Time::SetDeltaTime( double deltaTime )
	{
		Time::deltaTime = deltaTime;

		if ( !isTimePaused )
		{
			time += deltaTime * modifier;
		}
	}

	void Time::TimePaused( bool isTimePaused )
	{
		Time::isTimePaused = isTimePaused;
	}
}