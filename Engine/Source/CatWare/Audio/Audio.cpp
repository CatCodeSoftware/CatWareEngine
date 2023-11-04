#include "Audio.h"

#include "CatWare/Debug/Debug.h"
#include "CatWare/Utils/Log.h"

namespace CatWare
{
    
    // --------------------------------
    // Sound --------------------------
    // --------------------------------
    Sound::Sound( std::string filePath )
    {
        wave.load( filePath.c_str( ) );
    }

    // --------------------------------
    // AudioHandle --------------------
    // --------------------------------
    AudioHandle::AudioHandle( Sound* sound, int soloudHandle )
    {
        soundResource = sound;
        this->soloudHandle = soloudHandle;
    }

	void AudioHandle::Update( )
	{
		if ( isFinished )
			isFinished = AudioEngine::GetSoLoudInstance( )->isValidVoiceHandle( soloudHandle );
	}

    void AudioHandle::Pause( )
    {
        isPaused = !isPaused;
        AudioEngine::GetSoLoudInstance( )->setPause( soloudHandle, isPaused );
    }

    void AudioHandle::Stop( )
    {
        isFinished = true;
    }

    
    void AudioHandle::SetLooping( bool looping )
    {
        AudioEngine::GetSoLoudInstance( )->setLooping( soloudHandle, looping );
        this->looping = looping;
    }

	void AudioHandle::SetPan( float pan )
	{
		AudioEngine::GetSoLoudInstance( )->setPan( soloudHandle, pan );
		this->pan = pan;
	}

	void AudioHandle::SetVolume( float volume )
	{
		AudioEngine::GetSoLoudInstance( )->setVolume( soloudHandle, volume );
		this->volume = volume;
	}

	void AudioHandle::SetSpeed( float speed )
	{
		AudioEngine::GetSoLoudInstance( )->setRelativePlaySpeed( soloudHandle, speed );
		this->speed = speed;
	}

	// --------------------------------
    // AudioListener2D ----------------
    // --------------------------------
	AudioListener2D::AudioListener2D( Vector2D position, float volume )
	{
		this->position = position;
		this->volume = volume;
	}

	// --------------------------------
    // AudioHandle2D ------------------
    // --------------------------------
	AudioHandle2D::AudioHandle2D( Sound* sound, Vector2D position, float baseVolume, double radius, int soloudHandle ) : AudioHandle( sound, soloudHandle )
	{
		this->position = position;
		this->volumeCenter = baseVolume;
		this->radius = radius;
	}

	void AudioHandle2D::Update( )
	{
		// CW_ENGINE_LOG->Info( "Called" );

		// calculate volume
		/* get how far away the audio is from the listener and convert that into 0 - 1, 
		then multiply that by volume of the sound and listener */
		double distance = position.GetDistanceTo( AudioEngine::audioListener.position );

		if ( distance > radius )
			distance = radius;

		float baseVolume = ( radius - distance ) / radius;
		float finalVolume = baseVolume * AudioEngine::audioListener.volume * this->volumeCenter;

		SetVolume( finalVolume );

		double panDistance = 0;

		if ( AudioEngine::audioListener.position.x - position.x > 0 )
			panDistance = -distance;
		else if ( AudioEngine::audioListener.position.x - position.x < 0 )
			panDistance = distance;	

		float pan = panDistance / radius;
		SetPan( pan );
	}

    // --------------------------------
    // AudioEngine --------------------
    // --------------------------------
	AudioListener2D AudioEngine::audioListener( { 0, 0 }, 0 );

    std::vector<AudioHandle*> AudioEngine::audioHandles;
    SoLoud::Soloud AudioEngine::soloud;

    void AudioEngine::InitAudio( )
    {
        soloud.init( );
    }

    void AudioEngine::DeInitAudio( )
    {
        soloud.deinit( );

		for ( AudioHandle* audioHandle : audioHandles )
		{
			delete audioHandle;
		}

		audioHandles.clear( );
    }

    AudioHandle* AudioEngine::PlaySound( Sound* sound )
    {
        int soloudHandle = soloud.play( *( sound->GetWave( ) ), 1.0 );

        AudioHandle* audioHandle = new AudioHandle( sound, soloudHandle );
        audioHandles.push_back(audioHandle);

        if ( soloudHandle == -1 )
        {
            CW_ENGINE_LOG->Error( "Failed to play sound" );
        }

        return audioHandle;
    }

	AudioHandle2D* AudioEngine::PlaySound2D( Sound* sound, Vector2D position, float volume, double radius )
	{
		int soloudHandle = soloud.play( *( sound->GetWave( ) ), 1.0 );

        AudioHandle2D* audioHandle = new AudioHandle2D( sound, position, volume, radius, soloudHandle );
        audioHandles.push_back(audioHandle);

        if ( soloudHandle == -1 )
        {
            CW_ENGINE_LOG->Error( "Failed to play sound" );
        }

        return audioHandle;
	}

	void AudioEngine::UpdateHandles( )
	{
		for ( auto it = audioHandles.begin( ); it != audioHandles.end( ); )
		{
			( *it )->Update( );

			if ( ( *it )->IsFinished( ) )
			{
				delete *it;
				audioHandles.erase( it );
			}
			else
				it++;
		}
	}
}