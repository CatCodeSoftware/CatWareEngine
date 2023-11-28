#pragma once

#include <vector>
#include <string>

#include "CatWare/Core.h"
#include "CatWare/Types/Types.h"

namespace CatWare
{
	enum class FileMode
	{
		READ,
		WRITE,
		RW,
		READ_BINARY,
		WRITE_BINARY,
		RW_BINARY
	};
	
	class CATWARE_API FileHandle
	{
		friend class FileSystemSource;
		
	public:
		inline std::string GetPath( ) { return path; }
		inline bool IsBinary( ) { return isBinary; }
		
		virtual UInt64 GetSizeBytes( ) = 0;
		
		virtual char* Read( ) = 0;
		virtual void Write( char* data ) = 0;
		
	protected:
		FileHandle( bool isBinary, std::string path, FileMode mode );
	
		bool isBinary;
		FileMode mode;
		std::string path;
	};
	
	class CATWARE_API FileSystemSource
	{
	public:
		virtual std::string GetTitle( ) = 0;
		virtual bool IsReadOnly( ) = 0;
	
		virtual bool IsFile( std::string path ) = 0;
		virtual bool IsDir( std::string path ) = 0;
		
		virtual FileHandle* OpenFile( FileMode mode, std::string path ) = 0; 
		
		virtual std::vector<std::string> ListDir( std::string path ) = 0;
	};
	
	class CATWARE_API FileSystem
	{
	public:
		static void AddSource( FileSystemSource* source );
		static FileHandle* OpenFile( std::string path, FileMode mode, bool veryImportant = false );
		
	private:
		static std::vector<FileSystemSource*> sources;
	};
}