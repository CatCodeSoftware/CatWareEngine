#pragma once

#include "Utils/Types.h"
#include "Core.h"

namespace CatWare
{
	namespace Random
	{
		CATWARE_API void Init( );
		CATWARE_API void Seed( UInt64 seed );

		CATWARE_API Int64 GetInt( Int64 min, Int64 max );
		CATWARE_API UInt64 GetUInt( Int64 min, Int64 max );

		CATWARE_API float GetFloat( float min, float max );
		CATWARE_API double GetDouble( double min, double max );
	}
}