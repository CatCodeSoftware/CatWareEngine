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
			for ( int i = 0; i < memInfo.size( ); i++ )
			{
				if ( memInfo[i].address == pointer )
				{
					// We dont care about order here so we use a trick to remove the info
					// do a switcharoo with the last element and the element we want to delete
					MemInfo memInfo1 = memInfo[i]; // The element we want to delete
					MemInfo memInfo2 = memInfo[memInfo.size( ) - 1]; // The last element
				
					memInfo[i] = memInfo2;
					memInfo[memInfo.size( ) - 1] = memInfo1;

					// pop that sucker
					memInfo.pop_back( );

					break;
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