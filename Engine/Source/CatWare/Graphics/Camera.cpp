#include "Camera.h"

#include "glm/common.hpp"
#include "glm/ext.hpp"

#define RADS_PER_DEGREE 0.0174532925f

namespace CatWare
{
	OrthoCamera::OrthoCamera( int screenWidth, int screenHeight )
	{
		projection = glm::ortho( ( float ) 0, ( float ) screenWidth, ( float ) screenHeight, ( float ) 0 );
		RecalculateViewMatrix( );
	}

	void OrthoCamera::RecalculateViewMatrix( )
	{
		view = glm::translate( glm::mat4( 1.0f ), glm::vec3( focus.x, focus.y, 1 ) ) *
			glm::rotate( glm::mat4( 1.0f ), rotation * RADS_PER_DEGREE, glm::vec3( 0, 0, 1 ) );

		view = glm::scale( view, glm::vec3( 1 / scale, 1 / scale, 1 ) );

		view = glm::inverse( view );
	}

} // namespace CatWare
