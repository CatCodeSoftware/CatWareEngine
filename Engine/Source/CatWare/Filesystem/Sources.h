#pragma once

#include "Filesystem.h"
#include "CatWare/Utils/Types.h"

#include <cstdio>

namespace CatWare
{
	class CATWARE_API DirectorySource : public FileSystemSource
	{
	public:
		DirectorySource( std::string path );
		
		inline std::string GetTitle( ) override { return "Directory Source"; }
		inline bool IsReadOnly( ) override { return false; }
	
		bool IsFile( std::string path ) override;
		bool IsDir( std::string path ) override;
		
		FileHandle* OpenFile( FileMode mode, std::string path ) override; 
		
		std::vector<std::string> ListDir( std::string path ) override;
		 
	private:
		std::string rootPath;
	};
	
	class CATWARE_API StandardFileHandle : public FileHandle
	{
	public:
		StandardFileHandle( bool isBinary, std::string path, FileMode mode );
	
		UInt64 GetSizeBytes( ) override;
		
		char* Read( ) override;
		void Write( char* data ) override;
		
	private:
		FILE* fileHandle;
	};
} 