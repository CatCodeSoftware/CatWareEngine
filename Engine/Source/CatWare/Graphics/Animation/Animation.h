#pragma once

#include <string>
#include <vector>

#include "CatWare/Core.h"
#include "CatWare/Types/Color.h"
#include "CatWare/Types/Vector.h"

namespace CatWare
{
	class CATWARE_API Animation
	{
	public:
		Animation( std::string filePath );
		Animation( float fps, std::vector<std::string> frames);

		void Draw( Vector2D position, Vector2D size, Color color = { 255, 255, 255, 255 }, float rotation = 0 );

		unsigned int currentFrame = 0;
		float fps;
		bool paused = false;

	private:
		void Update( );

		std::vector<std::string> frames;

		double currentFrameStartTime = 0;
	};
}