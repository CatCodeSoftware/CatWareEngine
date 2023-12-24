#include "Particles.h"


#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "Assets/Assets.h"
#include "Graphics/Renderer/Renderer.h"
#include "Log.h"
#include "Random.h"
#include "Time.h"

namespace CatWare
{
	float Lerp(float v0, float v1, float t)
	{
		return v0 + t * (v1 - v0);
	}

	void Particle::Draw( )
	{
		Color color;

		float timeProgress = ( Time::GetTime( ) - startTime ) / ( endTime - startTime );

		color.r = Lerp( startColor.r, endColor.r, timeProgress );
		color.g = Lerp( startColor.g, endColor.g, timeProgress );
		color.b = Lerp( startColor.b, endColor.b, timeProgress );
		color.a = Lerp( startColor.a, endColor.a, timeProgress );

		Vector2D size = { 3, 3 };

		if ( velocityStrech )
			size.x += ( ( velocity.x + velocity.y ) / 2 ) / 20;

		if ( size.x < 3 ) size.x = 3;
		if ( size.y < 3 ) size.x = 3;

		if ( !textured )
			Renderer::DrawRect( position, size, color, angle );
		else
			Renderer::DrawRectTextured( position, size, Assets::textures.GetAsset( textureID ), color, angle );
	}

	void Particle::Update( )
	{
		// set rotation to next position
		angle = position.GetRotationTo( position + velocity * Time::GetDeltaTime( ) );
		velocity += gravity * Time::GetDeltaTime( );
		position += velocity * Time::GetDeltaTime( );
	}

	void ParticleEmmiter::Emit( )
	{
		if ( nextEmmit < Time::GetTime( ) && !( once && numEmmisions > 0 ) )
		{
			int particles = ( int ) numParticles + Random::GetInt( -( ( int ) numParticlesRandomness / 2 ), ( ( int ) numParticlesRandomness / 2 ) );

			if ( particles < 0 )
				particles = 0;

			if ( particles != 0 )
				this->particles.reserve( particles );

			for ( unsigned int i = 0; i < particles; i++ )
			{
				Particle part;

				part.position = position;
				part.velocity = Vector2D::Normalize( angle + Random::GetFloat( -spread / 2, spread / 2 ), speed + Random::GetFloat( -speedRandomness / 2, speedRandomness / 2 ) );
				part.gravity = gravity;

				part.startColor = startColor;
				part.endColor = endColor;

				part.startTime = Time::GetTime( );
				part.endTime = Time::GetTime( ) + lifetime + Random::GetFloat( -lifetimeRandomness / 2, lifetimeRandomness / 2 );

				part.textured = textured;

				if ( textured )
					part.textureID = textureIDs[ Random::GetUInt( 0, textureIDs.size( ) - 1 ) ];

				this->particles.push_back( part );
			}

			numEmmisions++;
			nextEmmit = Time::GetTime( ) + delay;
		}

		for ( unsigned int i = 0; i < particles.size( ); )
		{
			Particle& part = particles[i];

			part.Update( );

			if ( part.endTime < Time::GetTime( ) )
			{
				particles.erase( particles.begin( ) + i );
			}
			else
			{
				i++;
			}
		}
	}

	void ParticleEmmiter::Draw( )
	{
		for ( Particle& part : this->particles )
			part.Draw( );
	}
}
