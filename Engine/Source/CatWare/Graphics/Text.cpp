#include "Text.h"

#include "CatWare/Debug/Debug.h"

#include "CatWare/Log.h"
#include "Renderer/Texture.h"

namespace CatWare
{
	namespace Text
	{
		static FT_Library freetype;

		Character::~Character( )
		{
			delete texture;
		}

		Font::Font( std::string fontPath, unsigned int size )
		{
			FT_New_Face( freetype, fontPath.c_str( ), 0, &freetypeFace );

			FT_Set_Pixel_Sizes( freetypeFace, 0, size );

			// load all the glyphs
			for ( unsigned char ch = 0; ch < 128; ch++ )
			{
				if ( FT_Load_Char( freetypeFace, ch, FT_LOAD_RENDER ) )
				{
					CW_ENGINE_LOG->Error( "Failed to load character %c", ch );
				}

				Character* character = new Character;

				character->size = { double( freetypeFace->glyph->bitmap.width ), double( freetypeFace->glyph->bitmap.rows ) };
				character->bearing = { double( freetypeFace->glyph->bitmap_left ), double( freetypeFace->glyph->bitmap_top ) };
				character->advance = freetypeFace->glyph->advance.x;

				// Create texture with the character
				character->texture = Rendering::Texture2D::Create( freetypeFace->glyph->bitmap.width, freetypeFace->glyph->bitmap.rows, freetypeFace->glyph->bitmap.buffer, 1 );

				characters.insert( { ch, character } );
			}

			this->size = size;
		}

		float Font::GetStringSize( std::string string )
		{
			float size = 0;

			for ( char c : string )
			{
				if ( c == ' ' )
					size += spaceSize;

				else if ( c == '\t' )
					size += tabSize;

				else
					size += GetCharacter( c )->size.x + GetCharacter( c )->bearing.x;
			}

			return size;
		}

		Font::~Font( )
		{
			for ( auto keyVal : characters )
			{
				delete keyVal.second;
			}

			characters.clear( );
		}

		Character* Font::GetCharacter( char ch )
		{
			return characters[ch];
		}

		void InitFreetype( )
		{
			if ( FT_Init_FreeType( &freetype ) )
			{
				CW_ENGINE_LOG->Error( "Failed to initialize freetype" );
			}
		}
	}
}