#pragma once

#include <math.h>

class Vector2D
{
public:
	double x, y;

	inline double GetDistanceTo( Vector2D vec )
	{
		Vector2D newVec;

		newVec.x = fabs( x - vec.x );
		newVec.y = fabs( y - vec.y );

		return sqrt( pow( newVec.x, 2 ) + pow( newVec.y, 2 ) );
	}

	inline double GetRotationTo( Vector2D vec )
	{
		Vector2D directionVec = { vec.x - x, vec.y - y };
		directionVec.x = -directionVec.x;
		directionVec.y = -directionVec.y;

		double direction = atan2( directionVec.y, directionVec.x ) * 57.2957795;

		return direction;
	}

	inline static Vector2D Normalize( int rotation, double length )
	{
		double tangents = rotation / 57.2957795;

		return { length * cos( tangents ), length * sin( rotation ) };
	}
};
