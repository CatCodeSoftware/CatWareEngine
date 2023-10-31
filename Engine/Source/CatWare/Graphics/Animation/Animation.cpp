#include "Animation.h"

#include "CatWare/Debug/Debug.h"

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Resource/TextureResource.h"

namespace CatWare
{
	void Animation::Draw( Vector2D position, Vector2D scale, float rotation )
	{
		KeyFrame keyFrame = keyFrames[currentKeyFrame];

		Vector2D largestPosSize = { 0, 0 };

		for ( AnimationElement element : keyFrame.elements )
		{
			Vector2D posSize = element.transform.position + element.transform.size;

			if ( posSize.x > largestPosSize.x )
				largestPosSize.x = posSize.x;

			if ( posSize.y > largestPosSize.y )
				largestPosSize.y = posSize.y;
		}
		// render everything to a texture and then draw it
		// Get biggest position + size in elements
		Rendering::FrameBufferSpec fbSpec;
		fbSpec.width = largestPosSize.x * scale.x;
		fbSpec.height = largestPosSize.y * scale.y;

		Rendering::FrameBuffer* frameBuffer = Rendering::FrameBuffer::Create( fbSpec );

		Renderer::SetRenderTarget( frameBuffer );

		Renderer::Clear( { 0, 0, 0, 255 } );

		for ( AnimationElement element : keyFrame.elements )
		{
			if ( element.textured )
				Renderer::DrawRectTextured( element.transform.position, element.transform.size * scale, TextureManager::GetTexture( element.textureID ), element.color, element.transform.rotation );
			else
				Renderer::DrawRect( element.transform.position, element.transform.size * scale, element.color, element.transform.rotation );
		}

		Renderer::DrawRect( { 1, 1 }, { 20, 20 }, { 255, 255, 0, 255 } );

		Renderer::SetRenderTarget( nullptr );

		Renderer::DrawRectTextured( position,
			{ double( frameBuffer->GetColorAttachment( )->GetTextureWidth( ) ), double( frameBuffer->GetColorAttachment( )->GetTextureHeight( ) ) },
			frameBuffer->GetColorAttachment( ),
			Color( ),
			rotation
		);

		delete frameBuffer;
	}

	void Animation::Update( )
	{
		
	}
}