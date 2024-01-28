#include "World.h"

#include <yaml-cpp/yaml.h>

#include "CatWare/Log.h"
#include "CatWare/Filesystem/Filesystem.h"

namespace CatWare
{
	void World::Clean( )
	{
		entities.CleanUp( );
		physicsWorld.Clean( );
	}


	void World::LoadFromMapFile( const std::string& mapPath )
	{
		CW_ENGINE_LOG->Info( "Loading map from %s", mapPath.c_str( ) );

		// Load yaml file
		FileHandle *fileHandle = FileSystem::OpenFile( mapPath, FileMode::READ, true );
		char *content = fileHandle->Read( );

		YAML::Node mapFile = YAML::Load( content );

		auto baseNode = mapFile["map"].as< YAML::Node >( );

		YAML::Node entitiesNode = baseNode["entities"].as< YAML::Node >( );

		for ( YAML::Node entityNode : entitiesNode )
		{
			std::string className = entityNode["class"].as< std::string >( );

			YAML::Node transformNode = entityNode["transform"].as< YAML::Node >( );

			YAML::Node positionNode = transformNode["position"].as< YAML::Node >( );

			Vector2D positionVec = {
				positionNode["x"].as< double >( ),
				positionNode["y"].as< double >( )
			};

			YAML::Node sizeNode = transformNode["size"].as< YAML::Node >( );

			Vector2D sizeVec = {
				sizeNode["x"].as< double >( ),
				sizeNode["y"].as< double >( )
			};

			double rotation = transformNode["rotation"].as< double >( );

			int layer = transformNode["layer"].as< int >( );

			Transform finalTransform;
			finalTransform.position = positionVec;
			finalTransform.size = sizeVec;
			finalTransform.rotation = rotation;
			finalTransform.layer = layer;

			YAML::Node tagsNode = entityNode["tags"].as< YAML::Node >( );
			std::unordered_map< std::string, std::string > tags;

			for ( YAML::Node tag : tagsNode )
			{
				YAML::Node node = tag.as< YAML::Node >( );

				tags.insert( {node[0].as< std::string >( ), node[1].as< std::string >( )} );
			}

			entities.CreateEntity( className, finalTransform, tags );
		}
	}
}
