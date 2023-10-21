#pragma once

#include <string>
#include <unordered_map>

#include "Renderer/Texture.h"
#include "../Utils/Vector.h"
#include "CatWare/Core.h"

namespace CatWare
{
	namespace Text
	{
		class Character
		{
		public:
			~Character( );

			Rendering::Texture2D* texture;

			Vector2D size;
			Vector2D bearing;

			unsigned int advance;
		};

		class CATWARE_API Font
		{
		public:
			Font( std::string fontPath, unsigned int size );
			~Font( );

			Rendering::Texture2D* GetCharacterTexture( char ch );

		private:
			std::unordered_map<char, Character> characters;
		};
	}
}