#include "Animation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Assets/TextureAsset.h"
#include "CatWare/Time.h"


namespace CatWare
{
	Animation::Animation( std::string filePath )
	{
		// get all lines from file
		std::ifstream file( filePath );

		std::vector<std::string> lines;
		std::string buffer;

		while ( std::getline( file, buffer ) )
		{
			lines.push_back( buffer );
		}

		// read fps from first line
		fps = std::stof( lines[0] );

		for ( int i = 1; i < lines.size( ); i++ )
		{
			frames.push_back( lines[i] );
		}
	}

	Animation::Animation( float fps, std::vector<std::string> frames )
	{
		this->fps = fps;
		for ( int i = 0; i < frames.size( ); i++ )
		{
			this->frames[i] = TextureRef( frames[i] );
		}

		currentFrameStartTime = Time::GetDeltaTime( );
	}

	void Animation::Draw( Vector2D position, Vector2D size, Color color, float rotation )
	{
		Update( );

		Renderer::DrawRectTextured( position, size, frames[currentFrame].Get( ), color, rotation );
	}

	void Animation::Update( )
	{
		if ( Time::GetTime( ) - currentFrameStartTime > 1 / fps && !paused )
		{
			currentFrame++;

			if ( currentFrame >= frames.size( ) )
				currentFrame = 0;

			currentFrameStartTime = Time::GetTime( );
		}
	}
}