#pragma once

#include <string>
#include <vector>

#include "CatWare/Core.h"
#include "CatWare/Utils/Transform.h"
#include "CatWare/Utils/Color.h"

namespace CatWare
{
	class AnimationElement
	{
	public:
		Transform transform;

		bool textured = false;
		std::string textureID;

		Color color;
	};

	class KeyFrame
	{
	public:
		float nextKeyFrameDelay;
		std::vector<AnimationElement> elements;
	};

	class CATWARE_API Animation
	{
	public:
		inline void AddKeyFrame( KeyFrame keyFrame )
		{
			keyFrames.push_back( keyFrame );
		}

		void Draw( Vector2D position, Vector2D scale, float rotation );
		void Update( );

	private:
		double currentKeyFrameStartTime = 0;

		unsigned int currentKeyFrame = 0;
		std::vector<KeyFrame> keyFrames;
	};
}