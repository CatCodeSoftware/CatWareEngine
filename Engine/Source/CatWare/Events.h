#pragma once

#include <vector>

#include "CatWare/Core.h"

namespace CatWare
{
	enum class EventType
	{
		INPUT, WINDOW, QUIT
	};

	enum class InputEventType
	{
		MOUSE, KEYBOARD
	};

	enum class ButtonEventType
	{
		UP, DOWN
	};

	class CATWARE_API Event
	{
	public:
		EventType* eventType;
	};

	class CATWARE_API InputEvent : Event
	{
	public:
		InputEvent( InputEventType inputEventType, ButtonEventType buttonEventType, int button );

		InputEventType inputEventType;
		ButtonEventType buttonEventType;
		int buttonCode;
	};

	typedef void ( *EventHandler )( Event* event );

	class EventSystem
	{
	public:
		static void IssueEvent( Event* event );
		static void AddEventHandler( EventHandler* eventHandler );

	private:
		inline static std::vector< EventHandler* > eventHandlers;
	};
}