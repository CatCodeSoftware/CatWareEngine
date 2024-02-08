#pragma once

#include <string>
#include <vector>


#include "CatWare/Core.h"
#include "Types/Color.h"
#include "Types/Vector.h"
#include "Assets/TextureAsset.h"

namespace CatWare
{
	class CATWARE_API Particle
	{
	public:
		Vector2D position;
		Vector2D velocity;
		Vector2D size;
		Vector2D endSize;
		float airResistance;
		float angle;

		Color startColor;
		Color endColor;

		double startTime;
		double endTime;

		Vector2D gravity;

		bool velocityStrech;

		bool textured;
		std::string textureID;
		// this is a lil hacky. Idealy this would be stack allocated
		// But the new texture refrence system breaks when copying and the texture gets unloaded and loaded way too many times
		TextureRef* texture;

		void Draw( );
		void Update( );
	};

	class Particles
	{
	public:
		void ReserveSpace( unsigned int space );
		void AddParticle( Particle& part );

		void Draw( );

	private:
		std::vector< Particle > particles;
	};

	class CATWARE_API ParticleEmmiter
	{
	public:
		Vector2D position;

		Vector2D size = { 4, 4 };
		Vector2D endSize = { 0, 0 };

		float angle;
		float lifetime = 2;
		float lifetimeRandomness = 0;

		bool velocityStrech = false;

		float airResistance = 0;
		Vector2D gravity = { 0, 0 };
		float speed = 0;
		float speedRandomness = 0;

		bool once;
		double delay = 0;

		unsigned int numParticles = 0;
		unsigned int numParticlesRandomness = 0;

		bool textured = false;
		std::vector< std::string > textureIDs;

		Color startColor;
		Color endColor;
		// Color colorVariation = Color( 0, 0, 0, 0 ); // Not implemented in emmiter

		Vector2D particleVelocity;
		float velocityRandomness;
		float spread;

		void Emit( );

	private:
		double nextEmmit = 0;
		unsigned int numEmmisions = 0;
	};
}