#include "Animation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Resource/TextureResource.h"
#include "CatWare/Utils/Time.h"

namespace CatWare
{
	void Animation::Draw( Vector2D position, Vector2D scale, float rotation )
	{
		Update( );

		KeyFrame keyFrame = keyFrames[currentKeyFrame];

		// Get largest size + pos
		Vector2D largestPosSize = { 0, 0 };

		for ( AnimationElement element : keyFrame.elements )
		{
			Vector2D posSize = element.transform.position + element.transform.size;

			if ( posSize.x > largestPosSize.x )
				largestPosSize.x = posSize.x;

			if ( posSize.y > largestPosSize.y )
				largestPosSize.y = posSize.y;
		}

		for ( AnimationElement element : keyFrame.elements )
		{
			// Create a transform matrix
			glm::mat4 transform( 1.0 );

			// rotate it again by the origin of the whole frame
			transform = glm::translate( transform, glm::vec3( position.x + largestPosSize.x / 2, position.y + largestPosSize.y / 2, 0.f ) );
			transform = glm::rotate( transform, glm::radians( rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
			transform = glm::translate( transform, glm::vec3( -( position.x + largestPosSize.x / 2 ), -( position.y + largestPosSize.y / 2 ), 0.f ) );

			// first rotate the element around itself
			transform = glm::translate( transform, glm::vec3( position.x + element.transform.position.x + element.transform.size.x / 2, position.y + element.transform.position.y + element.transform.size.y / 2, 0.0f ) );
			transform = glm::rotate( transform, glm::radians( element.transform.rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
			transform = glm::translate( transform, glm::vec3( -( position.x + element.transform.position.x + element.transform.size.x / 2 ), -( position.y + element.transform.position.y + element.transform.size.y / 2 ), 0.0f ) );

			if ( element.textured )
				Renderer::DrawRectTextured( position + element.transform.position, element.transform.size * scale, TextureManager::GetTexture( element.textureID ), transform, element.color );
			else
				Renderer::DrawRect( position + element.transform.position, element.transform.size * scale, element.color, transform );
		}
	}

	void Animation::Update( )
	{
		if ( GlobalTime::GetTime( ) - currentKeyFrameStartTime > keyFrames[currentKeyFrame].nextKeyFrameDelay )
		{
			currentKeyFrame++;

			if ( currentKeyFrame >= keyFrames.size( ) )
				currentKeyFrame = 0;

			currentKeyFrameStartTime = GlobalTime::GetTime( );
		}
	}
}