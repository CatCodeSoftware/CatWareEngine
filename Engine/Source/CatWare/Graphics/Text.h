#pragma once

#include <string>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer/Texture.h"
#include "../Types/Vector.h"
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
			unsigned int spaceSize = 10;
			unsigned int tabSize = 40;

			Font( std::string fontPath, unsigned int size );
			~Font( );

			Character* GetCharacter( char ch );
			inline unsigned int GetSize( ) { return size; }

			float GetStringSize( std::string string );

		private:
			unsigned int size;
			FT_Face freetypeFace;

			std::unordered_map<char, Character*> characters;
		};

		CATWARE_API void InitFreetype( );
	}
}