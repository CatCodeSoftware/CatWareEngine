#pragma once

#include <string>
#include <vector>

#include "Time.h"

#ifdef CW_DEBUG
#define CW_PROFILE_FUNC CatWare::Profiling::Push( __FUNCTION__ )
#define CW_PROFILE_END CatWare::Profiling::Pop( )
#else
#define CW_PROFILE_FUNC
#define CW_PROFILE_END
#endif

namespace CatWare
{
	class ProfilingTimestamp
	{
	public:
		std::string name;
		InternalTimer timer;
		double endTime;
	};

	class Profiling
	{
	public:
		static void DrawProfilingPanel( );

		static void Push( std::string name );
		static void Pop( );
	private:
		inline static std::vector<ProfilingTimestamp> stamps;
		inline static int currentStamp = 0;
	};
}
