#include "Binds.h"

#include <unordered_map>
#include <fstream>

#include "CatWare/Debug/Debug.h"

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

		void LoadBindsFromConfig( std::string filePath )
		{
			// Load file
			std::ifstream file( filePath );

			std::string line;

			while ( getline ( file, line ) ) {
				// Output the text from the file
				// std::cout << line << std::endl;

				int stage = 0;

				std::string name;
				std::string inputType;
				std::string inputCode;

				// process the line char by char
				for ( char c : line )
				{
					if ( c != ' ' )
					{
						if ( c == '#' )
						{
							break;
						}
						else
						{
							if ( stage == 0 )
								name += c;
							else if ( stage == 1 )
								inputType += c;
							else if ( stage == 2 )
								inputCode += c;
						}
					}
					else
					{
						stage++;
					}
				}

				// std::cout << name << '\n' << inputType << '\n' << inputCode << '\n';
				if ( name != "" && inputType != "" && inputCode != "" )
				{
					int convertedCode = std::stoi( inputCode );

					if ( inputType == "keyboard" )
					{
						AddBinding( name, new KeyBind( convertedCode ) );
					}
					else if ( inputType == "mouse" )
					{
						AddBinding( name, new MouseBind( convertedCode ) );
					}
				}
			}
		}

		void CleanUpBinds( )
		{
			for ( auto bind : bindings )
			{
				delete bind.second;
			}
		}
	}
}