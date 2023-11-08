/**
* @file MemTracker.h
* 
* @brief Internal tool that tracks memory allocations in the engine
* but causes a lot of slowdown.
* 
* @author PointThink
*/

#pragma once

#include <vector>

#include "CatWare/Core.h"

namespace CatWare
{
	namespace Debug
	{
		struct MemInfo
		{
			size_t size;
			void* address;
			char const* file;
			int line;
		};

		class CATWARE_API MemoryTracker
		{
		public:
			static void AddMemory( MemInfo memInfo );
			static void RemoveMemory( void* pointer );

			static int GetCurrentUssage( );

			inline static std::vector<MemInfo> GetUnfreedMemory( )
			{
				return memInfo;
			}

		private:
			static std::vector<MemInfo> memInfo;
		};
	}
}

inline void* operator new( size_t size, char const* file, int line )
{
	void* pointer = malloc( size );
	CatWare::Debug::MemoryTracker::AddMemory( { size, pointer, file, line } );

	return pointer;
}

inline void operator delete( void* pointer )
{
	CatWare::Debug::MemoryTracker::RemoveMemory( pointer );

	free( pointer );
}

#define new new( __FILE__, __LINE__ )