#include "Animation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Assets/Assets.h"
#include "CatWare/Utils/Time.h"

namespace CatWare
{
	Animation::Animation( float fps, std::vector<std::string> frames )
	{
		this->fps = fps;
		this->frames = frames;

		currentFrameStartTime = GlobalTime::GetDeltaTime( );
	}

	void Animation::Draw( Vector2D position, Vector2D size, Color color, float rotation )
	{
		Update( );

		Renderer::DrawRectTextured( position, size, Assets::textures.GetAsset( frames[currentFrame] ), color, rotation );
	}

	void Animation::Update( )
	{
		if ( GlobalTime::GetTime( ) - currentFrameStartTime > 1 / fps && !paused )
		{
			currentFrame++;

			if ( currentFrame >= frames.size( ) )
				currentFrame = 0;

			currentFrameStartTime = GlobalTime::GetTime( );
		}
	}
}