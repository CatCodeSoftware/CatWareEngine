/**
* @file Vector.h
* 
* @brief Contains stuff related to vector math
* 
* @author PointThink
*/

#pragma once

#include <math.h>

#define VEC_PAIRWISE_OP( op ) inline Vector2D operator op(const Vector2D& otherVec) { return { x op otherVec.x, y op otherVec.y }; }
#define VEC_SINGLE_NUM_OP( op, numType )  inline Vector2D operator op(const numType& num) { return { x op num, y op num }; }
#define VEC_ASSIGN_OP( op ) inline void operator op(const Vector2D& otherVec) { x op otherVec.x; y op otherVec.y; }
#define VEC_ASSIGN_SN_OP( op, numType ) inline void operator op(const numType& num) { x op num; y op num; }

class Vector2D
{
public:
	inline Vector2D( )
	{
		x = 0;
		y = 0;
	}

	inline Vector2D( double x, double y )
	{
		this->x = x;
		this->y = y;
	}

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

	inline static Vector2D Normalize( float rotation, double length )
	{
		double tangents = rotation / 57.2957795;

		return { length * cos( tangents ), length * sin( tangents ) };
	}

	inline static double Dot( Vector2D vec1, Vector2D vec2 )
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	VEC_PAIRWISE_OP( + )
	VEC_PAIRWISE_OP( - )
	VEC_PAIRWISE_OP( * )
	VEC_PAIRWISE_OP( / )

	VEC_SINGLE_NUM_OP( +, float );
	VEC_SINGLE_NUM_OP( -, float );
	VEC_SINGLE_NUM_OP( *, float );
	VEC_SINGLE_NUM_OP( /, float );


	VEC_ASSIGN_OP( += )
	VEC_ASSIGN_OP( -= )
	VEC_ASSIGN_OP( *= )
	VEC_ASSIGN_OP( /= )

	VEC_ASSIGN_SN_OP( +=, float )
	VEC_ASSIGN_SN_OP( -=, float )
	VEC_ASSIGN_SN_OP( /=, float )
	VEC_ASSIGN_SN_OP( *=, float )
};

// dont spill over to the game
#undef VEC_PAIRWISE_OP
#undef VEC_ASSIGN_OP
#undef VEC_SINGLE_NUM_OP
#undef VEC_ASSIGN_SN_OP