#pragma once

#include <string>

#include "Core.h"

#define CW_ABORT(message) CatWare::FatalError( __FILE__, __LINE__, message )

namespace CatWare
{
	CATWARE_API void FatalError( std::string file, int line, std::string message );
}