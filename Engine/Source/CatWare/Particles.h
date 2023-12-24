#pragma once

#include <string>
#include <vector>


#include "CatWare/Core.h"
#include "Types/Color.h"
#include "Types/Vector.h"

namespace CatWare
{
	class CATWARE_API Particle
	{
	public:
		Vector2D position;
		Vector2D velocity;
		float angle;

		Color startColor;
		Color endColor;

		double startTime;
		double endTime;

		Vector2D gravity;

		bool velocityStrech;

		bool textured;
		std::string textureID;

		void Draw( );
		void Update( );
	};

	class CATWARE_API ParticleEmmiter
	{
	public:
		Vector2D position;
		float angle;
		float lifetime = 2;
		float lifetimeRandomness = 0;

		Vector2D gravity = { 0, 0 };
		float speed = 0;
		float speedRandomness = 0;

		bool once;
		double delay = 0;

		unsigned int numParticles = 0;
		unsigned int numParticlesRandomness = 0;

		bool textured = false;  // Not implemented in particle
		std::vector< std::string > textureIDs;  // Not implemented in particle

		Color startColor;
		Color endColor;
		Color colorVariation = Color( 0, 0, 0, 0 ); // Not implemented in emmiter

		Vector2D particleVelocity;
		float velocityRandomness;
		float spread;

		void Emit( );
		void Draw( );

	private:
		double nextEmmit = 0;
		unsigned int numEmmisions = 0;

		std::vector< Particle > particles;
	};
}