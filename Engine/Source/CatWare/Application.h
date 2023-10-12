#pragma once

#include "Core.h"

namespace CatWare
{
	class CATWARE_API Application
	{
	public:
		Application( );
		virtual ~Application( );

		virtual void Run( ) = 0;
	};
}