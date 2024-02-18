#include "Widgets.h"

#include "../Input/KeyboardAndMouse.h"
#include "CatWare/Graphics/Renderer/Renderer.h"

namespace CatWare
{
	namespace UI
	{
		bool Widget::IsHovered( )
		{
			Vector2D mousePos = Input::GetMouseMotion( );

			return (
				( mousePos.x > position.x && mousePos.y > position.y ) &&
				( mousePos.x < position.x + size.x && mousePos.y < position.y + size.y ) );
		}

		Button::Button( std::string text )
		{
			this->text = text;
			this->onPress = nullptr;
		}

		Button::Button( std::string text, void ( *onPress )( ) )
		{
			this->text = text;
			this->onPress = onPress;
		}

		void Button::Draw( )
		{
			Renderer::DrawRect( position, size, Style::buttonBorderColor );

			Vector2D innerRectPos = position + Style::buttonBorderWidth;
			Vector2D innerRectSize = size - Style::buttonBorderWidth * 2;

			if ( IsHovered( ) )
				Renderer::DrawRect( innerRectPos, innerRectSize, Style::buttonHoveredBG );
			else
				Renderer::DrawRect( innerRectPos, innerRectSize, Style::buttonBG );

			Vector2D textPosition = position;

			// casting these down to an int prevents some artifacting
			textPosition.y = ( int ) ( position.y + size.y / 2 - Style::font->GetSize( ) / 2 );
			textPosition.x = ( int ) ( position.x + size.x / 2 - Style::font->GetStringSize( text ) / 2 );

			Renderer::DrawString( text, textPosition, 1, Style::font, Style::buttonText );
		}

		void Button::Update( )
		{
			if ( IsHovered( ) && Input::IsMousePressed( 1 ) && onPress != nullptr )
			{
				if ( !pressedLastFrame )
					onPress( );

				pressedLastFrame = true;
			}
			else
				pressedLastFrame = false;
		}
	} // namespace UI
} // namespace CatWare
