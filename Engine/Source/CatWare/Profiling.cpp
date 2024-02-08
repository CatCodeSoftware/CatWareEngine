#include "Profiling.h"

#include <imgui.h>

#include "Error.h"
#include "Time.h"

namespace CatWare
{
	void Profiling::DrawProfilingPanel( )
	{
		for ( ProfilingTimestamp stamp : stamps )
		{
			ImGui::Text( ( stamp.name + " - %lf" ).c_str( ), stamp.endTime );
		}

		stamps.clear( );
	}

	void Profiling::Push( std::string name )
	{
		ProfilingTimestamp pushedStamp;
		pushedStamp.name = name;
		pushedStamp.timer.Reset( );
		stamps.push_back( pushedStamp );
		currentStamp++;
	}

	void Profiling::Pop( )
	{
		ProfilingTimestamp& poppedStamp = stamps[currentStamp];
		poppedStamp.endTime = poppedStamp.timer.GetTime( );
		currentStamp--;
	}
}
