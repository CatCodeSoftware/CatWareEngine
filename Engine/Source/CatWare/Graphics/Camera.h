#pragma once

#include <glm/glm.hpp>

#include "CatWare/Core.h"
#include "CatWare/Types/Vector.h"

namespace CatWare
{
	class Camera
	{
	public:
		virtual glm::mat4 CalculateProjectionMatrix( ) = 0;
	};

	class CATWARE_API OrthoCamera : public Camera
	{
	public:
		OrthoCamera( int screenWidth, int screenHeight );

		glm::mat4 CalculateProjectionMatrix( ) override;

		void SetFocus( Vector2D focus );

	private:
		int renderOffsetX = 0;
		int renderOffsetY = 0;

		int screenWidth = 0;
		int screenHeight = 0;
	};
}