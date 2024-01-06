/**
* @file Console.h
*
* @brief The developer console in the debug utilities
*
* @author PointThink
*/

#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "CatWare/Core.h"

namespace CatWare
{
	class ConsoleCommand;

	enum class ConVarType
	{
		UINT_64, INT_64, UINT_32, INT_32,
		FLOAT, BOOL, STRING
	};

	/**
	* Short for Console Variable.
	* This allows variables to be changed through the developer console
	*
	* Eg. "set variable 10"
	*/
	class ConVar
	{
	public:
		ConVar( );
		ConVar( const ConVarType& type, void* valuePtr );

		inline bool IsValid( ) { return isValid; }
		inline ConVarType GetType( ) { return type; }
		inline void* GetValuePointer( ) { return valuePtr; }

	private:
		bool isValid;
		ConVarType type;
		void* valuePtr;
	};

	enum class ConsoleColor
	{
		RED, WHITE, ORANGE
	};

	class ConsoleText
	{
	public:
		std::string text;
		ConsoleColor color;
	};

	class CATWARE_API Console
	{
	public:
		inline static bool scrollToBottom = true;
		inline static bool autoScroll = true;

		static void Init( );

		static inline void AddCommand( ConsoleCommand* command ) { commands.push_back( command ); }
		static void RunString( std::string line );

		static inline void RegisterConVar( std::string name, ConVar convar ) { conVars.insert( { name, convar } ); }
		static inline ConVar GetConVar( std::string name ) { return conVars[name]; }

		static void Draw( );
		static inline void SetColor( ConsoleColor color ) { currentColor = color; }
		static inline void Print( std::string text ) { consoleContents.push_back( ConsoleText { text, currentColor } ); };

	private:
		inline static ConsoleColor currentColor = ConsoleColor::WHITE;

		inline static char commandBuffer[256];
		inline static std::vector<ConsoleText> consoleContents;

		inline static std::vector<ConsoleCommand*> commands;
		inline static std::unordered_map<std::string, ConVar> conVars;
	};

	class CATWARE_API ConsoleCommand
	{
	public:
		ConsoleCommand( std::vector<std::string> names, unsigned int requiredArgsCount );

		virtual void Run( std::vector<std::string> args ) = 0;

		inline std::vector<std::string> GetNames( ) { return names; }
		inline unsigned int GetRequiredArgs( ) { return requiredArgsCount; }

	private:
		std::vector<std::string> names;
		unsigned int requiredArgsCount;
	};
}