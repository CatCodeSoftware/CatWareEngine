#include "Commands.h"

#include "CatWare/Log.h"
#include "CatWare/Types/Types.h"

namespace CatWare
{
	namespace Commands
	{
		bool StringToBool( std::string string )
		{
			if ( string == "true" || string == "1" )
				return true;
			if ( string == "false" || string == "0" )
				return false;
		}

		Exit::Exit( ) : ConsoleCommand( { "exit", "quit" }, 0 ) {}
		void Exit::Run( std::vector<std::string> args ) { exit( 0 ); }

		Set::Set( ) : ConsoleCommand( { "set", "var" }, 2 ) {}

		void Set::Run( std::vector<std::string> args )
		{
			// Get convar
			ConVar conVar = Console::GetConVar( args[1] );

			if ( !conVar.IsValid( ) )
			{
				CW_ENGINE_LOG->Error( "Cannot set convar %s becaus it does not exist", args[1].c_str( ) );
				return;
			}

			CW_ENGINE_LOG->Info( "convar %s = %s", args[1].c_str( ), args[2].c_str( ) );

			// This is ugly but works so fuck it
			// It's probably faster to do all of this
			// instead of fucking around with templates
			switch ( conVar.GetType( ) )
			{
				case ConVarType::UINT_64: *( ( UInt64* ) conVar.GetValuePointer( ) ) = std::stoull( args[2] ); return;
				case ConVarType::INT_64: *( ( Int64* ) conVar.GetValuePointer( ) ) = std::stoll( args[2] ); return;
				case ConVarType::UINT_32: *( ( UInt64* ) conVar.GetValuePointer( ) ) = std::stoul( args[2] ); return;
				case ConVarType::INT_32: *( ( Int64* ) conVar.GetValuePointer( ) ) = std::stol( args[2] ); return;
				case ConVarType::BOOL: *( ( bool* ) conVar.GetValuePointer( ) ) = StringToBool( args[2] ); return;
				case ConVarType::FLOAT: *( ( float* ) conVar.GetValuePointer( ) ) = std::stof( args[2] ); return;
				case ConVarType::STRING: *( ( std::string* ) conVar.GetValuePointer( ) ) = args[2]; return;
			}
		}
	}
}