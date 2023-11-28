#include "Filesystem.h"

#include "CatWare/Utils/Log.h"
#include "CatWare/Error.h"

namespace CatWare
{
	// -----------------------------------
	// FileHandle
	// -----------------------------------
	FileHandle::FileHandle( bool isBinary, std::string path, FileMode mode )
	{
		this->isBinary = isBinary;
		this->path = path;
		this->mode = mode;
	}
	
	// -----------------------------------
	// FileSystem
	// -----------------------------------
	std::vector<FileSystemSource*> FileSystem::sources;
	
	void FileSystem::AddSource( FileSystemSource* source )
	{
		sources.push_back( source );
	}
	
	FileHandle* FileSystem::OpenFile( std::string path, FileMode mode, bool veryImportant )
	{
		// search the sources for a valid file
		for ( FileSystemSource* source : sources )
		{
			if ( source->IsFile( path ) )
			{
				return source->OpenFile( mode, path );
			}
		}
		
		if ( !veryImportant )
			CW_ENGINE_LOG->Error( "Could not load file %s", path.c_str( ) );
		else
			CW_ABORT( "Could not load file " + path );
		
		return nullptr;
	}
}