#pragma once

#include <string>

#include "CatWare/Core.h"
#include "CatWare/Utils/Vector.h"
#include "CatWare/Graphics/Renderer/Texture.h"

namespace CatWare
{
	namespace GUI
	{
		namespace Widgets
		{
			// base class for all widgets
			class Widget
			{
			public:
				virtual void Draw( ) { }
				virtual void Update( ) { }

				virtual void OnClick( unsigned int button ) { }
				virtual void OnType( unsigned int keyCode, char character ) { }

				Vector2D position;
				Vector2D size;
			};

			class Label
			{
			public:
				Label( std::string text );

				std::string text;
			};

			class Image
			{
			public:
				Image( Rendering::Texture2D* texture, Vector2D size );

				Rendering::Texture2D* texture;
			};

			class Button : public Widget
			{
			public:
				Button( std::string label, void ( *onClick )( ) = nullptr );

				void Update( ) override;
				void Draw( ) override;

				Rendering::Texture2D* buttonImage;
				std::string label;

				void ( *onClick )( ) = nullptr; // click callback
			};

			class Checkbox : public Widget
			{
			public:
				Checkbox( std::string label );

				void Update( ) override;
				void Draw( ) override;

				bool isChecked = false;
				std::string label;
			};

			class TextInput : public Widget
			{
			public:
				TextInput( );

				void Draw( ) override;
				void Update( ) override;

				void OnType( unsigned int keyCode, char character ) override;

				std::string GetString( );
				void SetString( std::string string );

				unsigned int GetCursorPos( );
				void SetCursorPos( );

				void ( *onEnter )( std::string string ) = nullptr;
				void ( *onCharacterTyped )( char character ) = nullptr;

			private:
				std::string contents;

				unsigned int cursorPos = 0;
			};

			class Slider
			{
			public:
				Slider( double minValue, double maxValue );

				void ( *onChanged )( double value ) = nullptr;

				double percent = 0;
			};
		}
	}
}