#pragma once

#include "CatWare/Core.h"
#include "CatWare/Graphics/Text.h"
#include "CatWare/Types/Color.h"
#include "CatWare/Types/Vector.h"

namespace CatWare
{
	namespace UI
	{
		class Style
		{
		public:
			// Text
			inline static Text::Font* font = nullptr;

			// buttons
			inline static Color buttonBG = { 40, 40, 40, 255 };
			inline static Color buttonHoveredBG = { 80, 80, 80, 255 };
			inline static Color buttonText = { 255, 255, 255, 255 };
			inline static int buttonBorderWidth = 2;
			inline static Color buttonBorderColor = { 255, 255, 255, 255 };
		};

		class CATWARE_API Widget
		{
		public:
			Vector2D position;
			Vector2D size;

			virtual void Draw( ) = 0;
			virtual void Update( ) = 0;

			virtual void OnPress( UInt8 button ) {}
			virtual void OnType( char character, int keycode ) {}

			bool IsHovered( );
		};

		class CATWARE_API Button : public Widget
		{
		public:
			std::string text;
			void ( *onPress )( ) = nullptr;
			bool pressedLastFrame = false;

			Button( std::string text );

			Button( std::string text, void ( *onPress )( ) );
			void Draw( ) override;
			void Update( ) override;
		};
	} // namespace UI
} // namespace CatWare
