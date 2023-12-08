#include "World.h"

#include <yaml-cpp/yaml.h>

#include "Log.h"
#include "Filesystem/Filesystem.h"

namespace CatWare
{

	void World::LoadFromMapFile(const std::string &mapPath)
	{
		// Load yaml file
		FileHandle* fileHandle = FileSystem::OpenFile( mapPath, FileMode::READ, true );
		char* content = fileHandle->Read( );

		YAML::Node mapFile = YAML::Load( content );

		CW_ENGINE_LOG->Error( "%d", mapFile["map"]["testEntity"]["position"]["x"].as<int>( ) );
	}
}
