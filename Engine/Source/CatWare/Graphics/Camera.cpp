#include "Camera.h"

#include "glm/common.hpp"
#include "glm/ext.hpp"

namespace CatWare
{
	OrthoCamera::OrthoCamera( int screenWidth, int screenHeight )
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		SetFocus( { double( screenWidth - screenWidth / 2 ), double( screenHeight - screenHeight / 2 ) } );
	}

	glm::mat4 OrthoCamera::CalculateProjectionMatrix( )
	{
		glm::mat4 proj = glm::ortho( float( -renderOffsetX ), float( screenWidth - renderOffsetX ), float( screenHeight - renderOffsetY ), float( -renderOffsetY ) );
		return proj;
	}

	void OrthoCamera::SetFocus( Vector2D focus )
	{
		renderOffsetX = screenWidth / 2 - focus.x;
		renderOffsetY = screenHeight / 2 - focus.y;
	}
}