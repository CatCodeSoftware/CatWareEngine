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
		return GetTime( ) / time;
	}

	void InternalTimer::Reset( )
	{
		startTime = std::chrono::high_resolution_clock::now( );
	}


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