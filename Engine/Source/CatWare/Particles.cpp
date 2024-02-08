#include "Particles.h"


#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "Assets/TextureAsset.h"
#include "Graphics/Renderer/Renderer.h"
#include "Log.h"
#include "Random.h"
#include "GameState/Scene.h"
#include "Time.h"

namespace CatWare
{
	inline static float Lerp( float v0, float v1, float t )
	{
		return v0 + t * ( v1 - v0 );
	}

	void Particle::Draw( )
	{
		Color color;

		float timeProgress = ( Time::GetTime( ) - startTime ) / ( endTime - startTime );

		color.r = Lerp( startColor.r, endColor.r, timeProgress );
		color.g = Lerp( startColor.g, endColor.g, timeProgress );
		color.b = Lerp( startColor.b, endColor.b, timeProgress );
		color.a = Lerp( startColor.a, endColor.a, timeProgress );

		Vector2D tempSize;
		tempSize.x = Lerp( size.x, endSize.x, timeProgress );
		tempSize.y = Lerp( size.y, endSize.y, timeProgress );

		if ( velocityStrech )
		{
			tempSize.x = ( ( velocity.x + velocity.y ) / 2 ) / 20;
		}

		if ( !textured )
			Renderer::DrawRect( position, tempSize, color, angle );
		else
			Renderer::DrawRectTextured( position - tempSize / 2, tempSize, texture->Get( ), color, angle );
	}

	void Particle::Update( )
	{
		// set rotation to next position
		angle = position.GetRotationTo( position + velocity * Time::GetDeltaTime( ) );
		velocity += gravity * Time::GetDeltaTime( );
		position += velocity * Time::GetDeltaTime( );

		velocity -= ( velocity * 10 * airResistance ) * Time::GetDeltaTime( );
	}

	void ParticleEmmiter::Emit( )
	{
		if ( nextEmmit < Time::GetTime( ) && !( once && numEmmisions > 0 ) )
		{
			int particles = ( int ) numParticles +
				Random::GetInt( -( ( int ) numParticlesRandomness / 2 ), ( ( int ) numParticlesRandomness / 2 ) );

			if ( particles < 0 )
				particles = 0;

			if ( particles != 0 )
				SceneManager::GetScene( )->world.particles.ReserveSpace( particles );

			for ( unsigned int i = 0; i < particles; i++ )
			{
				Particle part;

				part.angle = 0;
				part.velocityStrech = velocityStrech;
				part.airResistance = airResistance;

				part.position = position;
				part.velocity = Vector2D::Normalize(
					angle + Random::GetFloat( -spread / 2, spread / 2 ),
					speed + Random::GetFloat( -speedRandomness / 2, speedRandomness / 2 )
					);
				part.gravity = gravity;

				part.startColor = startColor;
				part.endColor = endColor;

				part.startTime = Time::GetTime( );
				part.endTime =
					Time::GetTime( ) + lifetime + Random::GetFloat( -lifetimeRandomness / 2, lifetimeRandomness / 2 );

				part.size = size;
				part.endSize = endSize;

				part.textured = textured;

				if ( textured )
					part.texture = new TextureRef( textureIDs[Random::GetUInt( 0, textureIDs.size( ) - 1 )] );

				SceneManager::GetScene( )->world.particles.AddParticle( part );
			}

			numEmmisions++;
			nextEmmit = Time::GetTime( ) + delay;
		}
	}

	void Particles::ReserveSpace( unsigned space )
	{
		particles.reserve( space );
	}

	void Particles::AddParticle( Particle& part )
	{
		particles.push_back( part );
	}

	void Particles::Draw( )
	{
		// TODO: Multithread this MF

		// delete dead particles
		for ( unsigned int i = 0; i < particles.size( ); i++ )
		{
			if ( particles[i].endTime < Time::GetTime( ) )
			{
				delete particles[i].texture;
				particles.erase( particles.begin( ) + i );
			}
		}

		for ( int i = particles.size( ) - 1; i >= 0; i-- )
		{
			Particle& part = particles[i];

			part.Update( );
			part.Draw( );
		}
	}
}
