#pragma once

#include "Entity.h"

#include "CatWare/Types/Color.h"
#include "CatWare/Types/Transform.h"

namespace CatWare
{
	class BasicEntityInfo
	{
		BasicEntityInfo( std::string name, std::vector<std::string> groups );

		std::string name;
		std::string uniqueName;
		std::vector<std::string> groups;
	};

	class SpriteRenderer
	{
	public:
		SpriteRenderer( Color color, std::string texture = "" )
		{
			this->color = color;
			this->texture = texture;

			if ( texture != "" )
			{
				isTextured = true;
			}
		}

		void Draw( Transform transform );

		bool isTextured = false;
		std::string texture;
		Color color;
	};
}