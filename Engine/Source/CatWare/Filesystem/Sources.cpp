#include "Sources.h"

#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;

namespace CatWare
{
	// -----------------------------------
	// DirectorySource
	// -----------------------------------
	DirectorySource::DirectorySource( std::string path )
	{
		rootPath = path;
	}
	
	bool DirectorySource::IsFile( std::string path )
	{
		return fs::is_regular_file( fs::status( fs::absolute( rootPath + "/" + path ) ) );
	}
	
	bool DirectorySource::IsDir( std::string path )
	{
		return false;
	}
	
	FileHandle* DirectorySource::OpenFile( FileMode mode, std::string path )
	{
		bool isBinary = false;
		
		if ( mode == FileMode::READ_BINARY || mode == FileMode::WRITE_BINARY || mode == FileMode::RW_BINARY )
			isBinary = true;
		
		return new StandardFileHandle( isBinary, rootPath + "/" + path, mode );
	}
	
	std::vector<std::string> DirectorySource::ListDir( std::string path ) 
	{
		std::vector<std::string> entries;
		std::string searchPath = rootPath + "/" + path;
		
		for (const auto& entry : fs::directory_iterator( searchPath ))
		{
			entries.push_back( entry.path( ).string( ) );
		}
		
		return entries;
	}
	
	// -----------------------------------
	// Standard file handle
	// -----------------------------------
	StandardFileHandle::StandardFileHandle( bool isBinary, std::string path, FileMode mode ) :
	FileHandle( isBinary, path, mode )
	{
		std::string cmode;

		if ( mode == FileMode::READ || mode == FileMode::READ_BINARY )
			cmode = "r";
		else if ( mode == FileMode::WRITE || mode == FileMode::WRITE_BINARY )
			cmode = "w";
		if ( mode == FileMode::RW || mode == FileMode::RW_BINARY )
			cmode = "r+";

		if ( isBinary )
			cmode += "b";
		
		fileHandle = fopen( path.c_str( ), cmode.c_str( ) );
	}
	
	UInt64 StandardFileHandle::GetSizeBytes( )
	{
		fseek( fileHandle, 0L, SEEK_END );
		UInt64 size = ftell( fileHandle ) + 1;
		rewind( fileHandle );

		return size;
	}
	
	char* StandardFileHandle::Read( )
	{
		UInt64 size = GetSizeBytes( );
		char* buffer = new char[size + 1];

		fseek( fileHandle, 0, SEEK_SET );
		fread( buffer, 1, size, fileHandle );
		
		return buffer;
	}
	
	void StandardFileHandle::Write( char* data )
	{
		// Todo: implement
	}
}