#include "Random.h"

#include <random>

namespace CatWare
{
	namespace Random
	{
		static std::mt19937 random;

		void Init( )
		{
			std::random_device randDevice;
			random = std::mt19937( randDevice( ) );
		}

		void Seed( UInt64 seed ) 
		{
			random.seed( seed );
		}

		Int64 GetInt( Int64 min, Int64 max )
		{
			std::uniform_int_distribution<Int64> dist( min, max );
			return dist( random );
		}

		UInt64 GetUInt( UInt64 min, UInt64 max )
		{
			std::uniform_int_distribution<UInt64> dist( min, max );
			return dist( random );
		}

		float GetFloat( float min, float max )
		{
			std::uniform_real_distribution<float> dist( min, max );
			return dist( random );
		}

		double GetDouble( double min, double max )
		{
			std::uniform_real_distribution<double> dist( min, max );
			return dist( random );
		}
	}
}