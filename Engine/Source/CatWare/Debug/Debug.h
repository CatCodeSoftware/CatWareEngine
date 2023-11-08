/**
* @file Debug.h
* 
* @brief Internal header that includes other headers
* if debugging is enabled.
* 
* @author PointThink
*/

#pragma once

#ifdef CW_DEBUG
#	include "MemTracker.h"
#endif