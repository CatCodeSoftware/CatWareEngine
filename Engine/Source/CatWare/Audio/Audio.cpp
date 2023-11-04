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
    // AudioEngine --------------------
    // --------------------------------
    std::vector<AudioHandle*> AudioEngine::audioHandles;
    SoLoud::Soloud AudioEngine::soloud;

    void AudioEngine::InitAudio( )
    {
        soloud.init( );
    }

    void AudioEngine::DeInitAudio( )
    {
        soloud.deinit( );
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
}