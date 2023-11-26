#include "Components.h"

#include "CatWare/Graphics/Renderer/Renderer.h"
#include "CatWare/Assets/Assets.h"

namespace CatWare
{
	BasicEntityInfo::BasicEntityInfo( std::string name, std::vector<std::string> groups )
	{
		this->name = name;
		this->groups = groups;
	}

	void SpriteRenderer::Draw( Transform transform )
	{
		if ( isTextured )
		{
			Renderer::DrawRectTextured( transform.position, transform.size, Assets::textures.GetAsset( texture ), color );
		}
	}
}