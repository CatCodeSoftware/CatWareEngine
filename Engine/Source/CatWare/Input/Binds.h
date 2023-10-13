#pragma once

#include <string>

#include "CatWare/Core.h"

namespace CatWare
{
	namespace Input
	{
		class CATWARE_API Binding
		{
		public:
			virtual bool IsDown( ) = 0;
			virtual float GetPercentDown( ) = 0; // For supporting pressure sensitive buttons in the future, maybe those weird pressure sensitive keyboard will be popular in the future
	 	};

		class CATWARE_API KeyBind : public Binding
		{
			unsigned int keyCode;

		public:
			KeyBind( unsigned int keyCode );

			bool IsDown( ) override;
			float GetPercentDown( ) override;
		};

		class CATWARE_API MouseBind : public Binding
		{
			unsigned int keyCode;

		public:
			MouseBind( unsigned int keyCode );

			bool IsDown( ) override;
			float GetPercentDown( ) override;
		};

		CATWARE_API void AddBinding( std::string label, Binding* binding );
		CATWARE_API Binding* GetBinding( std::string label );
		CATWARE_API void RemoveBinding( std::string label );
	}
}