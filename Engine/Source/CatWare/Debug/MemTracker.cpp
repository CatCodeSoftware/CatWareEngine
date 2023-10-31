#include "MemTracker.h"

namespace CatWare
{
	namespace Debug
	{
		std::vector<MemInfo> MemoryTracker::memInfo;

		void MemoryTracker::AddMemory( MemInfo memInfo )
		{
			MemoryTracker::memInfo.push_back( memInfo );
		}

		void MemoryTracker::RemoveMemory( void* pointer )
		{
			for ( auto it = memInfo.begin( ); it != memInfo.end( ); )
			{
				if ( it->address == pointer )
				{
					memInfo.erase( it );
					break;
				}
				else
				{
					it++;
				}
			}
		}

		int MemoryTracker::GetCurrentUssage( )
		{
			int ussage = 0;

			for ( MemInfo& mi : memInfo )
			{
				ussage += mi.line;
			}

			return ussage;
		}
	}
}