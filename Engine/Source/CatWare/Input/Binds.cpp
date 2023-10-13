#include "Binds.h"

#include <unordered_map>

#include "KeyboardAndMouse.h"

namespace CatWare
{
	namespace Input
	{
		// KEYBIND
		KeyBind::KeyBind( unsigned int keyCode )
		{
			this->keyCode = keyCode;
		}

		bool KeyBind::IsDown( )
		{
			return IsKeyPressed( keyCode );
		}

		float KeyBind::GetPercentDown( )
		{
			return IsKeyPressed( keyCode );
		}

		// MOUSEBIND
		MouseBind::MouseBind( unsigned int keyCode )
		{
			this->keyCode = keyCode;
		}

		bool MouseBind::IsDown( )
		{
			return IsMousePressed( keyCode );
		}

		float MouseBind::GetPercentDown( )
		{
			return IsMousePressed( keyCode );
		}


		// BIND MANAGER
		std::unordered_map<std::string, Binding*> bindings;

		void AddBinding( std::string label, Binding* binding )
		{
			bindings.insert( { label, binding } );
		}

		Binding* GetBinding( std::string label )
		{
			return bindings[label];
		}

		void RemoveBinding( std::string label )
		{
			delete bindings[label];
			bindings.erase( label );
		}
	}
}