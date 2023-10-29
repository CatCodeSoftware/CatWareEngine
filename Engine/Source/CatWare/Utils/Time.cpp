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

	
	double GlobalTime::time = 0;

	double GlobalTime::deltaTime = 0;
	double GlobalTime::modifier = 1.0;

	bool GlobalTime::isTimePaused = false;

	void GlobalTime::SetDeltaTime( double deltaTime )
	{
		GlobalTime::deltaTime = deltaTime;

		if ( !isTimePaused )
		{
			time += deltaTime * modifier;
		}
	}

	void GlobalTime::TimePaused( bool isTimePaused )
	{
		GlobalTime::isTimePaused = isTimePaused;
	}
}