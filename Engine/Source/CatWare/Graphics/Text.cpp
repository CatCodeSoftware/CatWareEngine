#include "Text.h"

namespace CatWare
{
	namespace Text
	{
		Character::~Character( )
		{
			delete texture;
		}

		Font::Font( std::string fontPath, unsigned int size )
		{

		}

		Font::~Font( )
		{
			characters.clear( );
		}

		Rendering::Texture2D* Font::GetCharacterTexture( char ch )
		{
			return characters[ch].texture;
		}
	}
}