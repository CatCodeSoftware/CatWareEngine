#include "KeyboardAndMouse.h"

namespace CatWare
{
	namespace Input
	{
		static bool keyStatus[512];
		static bool mouseStatus[16]; // I dont actually know how many mouse buttons are allowed, 16 seems good for now - PT

		static Vector2D mouseMotion; // If i recall corectly the mouse motion in sdl2 can be both its positon or movement. Depends if cursor is locked - PT

		void SetKeyPressed( int keyCode, bool isPressed )
		{
			keyStatus[keyCode] = isPressed;
		}

		void SetMousePressed( int buttonCode, bool isPressed )
		{
			mouseStatus[buttonCode] = isPressed;
		}


		bool IsKeyPressed( int keyCode )
		{
			return keyStatus[keyCode];
		}

		bool IsMousePressed( int buttonCode )
		{
			return mouseStatus[buttonCode];
		}


		void SetMouseMotion( Vector2D motion )
		{
			mouseMotion = motion;
		}

		Vector2D GetMouseMotion( )
		{
			return mouseMotion;
		}
	}
}