#include "Time.h"

#include <cmath>

namespace CatWare
{
	double Timer::GetTime( )
	{
		std::chrono::duration<float> fsec = std::chrono::high_resolution_clock::now( ) - startTime;
		return fsec.count( );
	}

	bool Timer::HasTimeElapsed( float time )
	{
		if ( GetTime( ) >= time )
		{
			return true;
		}

		return false;
	}

	int Timer::TimesTimeElapsed( float time )
	{
		return std::floor( GetTime( ) / time );
	}

	void Timer::Reset( )
	{
		startTime = std::chrono::high_resolution_clock::now( );
	}
}