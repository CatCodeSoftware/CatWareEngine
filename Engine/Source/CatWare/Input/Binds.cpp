#include "Binds.h"

#include <unordered_map>

#include "CatWare/Debug/Debug.h"

#include "CatWare/Error.h"
#include "CatWare/Filesystem/Filesystem.h"
#include "CatWare/Log.h"
#include "KeyboardAndMouse.h"
#include "SDL.h"

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
			FileHandle* handle = FileSystem::OpenFile( filePath, FileMode::READ, true );

			std::string contents = handle->Read( );

			// split contents into lines
			std::vector<std::string> lines;
			std::string buffer = "";

			for ( char c : contents )
			{
				if ( c == '\n' )
				{
					lines.push_back( buffer );
					buffer = "";
				}
				else
					buffer += c;
			}

			lines.push_back( buffer );

			for  ( std::string& line : lines )
			{
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

				if ( name != "" && inputType != "" && inputCode != "" )
				{
					int convertedCode;

					if ( inputType == "keyboard" )
					{
						convertedCode  = SDL_GetScancodeFromName( inputCode.c_str( ) );

						if ( convertedCode == SDL_SCANCODE_UNKNOWN  )
							CW_ENGINE_LOG->Error( "Unknown key name %s", inputCode.c_str( ) );

						AddBinding( name, new KeyBind( convertedCode ) );
					}
					else if ( inputType == "mouse" )
					{
						convertedCode = std::stoi( inputCode );
						AddBinding( name, new MouseBind( convertedCode ) );
					}
					else
						CW_ABORT( "Unknown input type " + inputType );
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
