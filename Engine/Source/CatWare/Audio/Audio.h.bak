/**
* @file Audio.h
* 
* @brief The entire sound system
* 
* @author PointThink
*/

#pragma once

#include <string>
#include <vector>

#include <soloud.h>
#include <soloud_wav.h>

#include "CatWare/Core.h"
#include "CatWare/Types/Vector.h"

namespace CatWare
{
	/**
	* A sound resource, not recomended to use this directly
	* Use the asset managers in Assets.h
	*/
    class CATWARE_API Sound
    {
	public:
		Sound( std::string soundFilePath );

		int useCount;

		inline SoLoud::Wav* GetWave( ) { return &wave; }
	private:
		SoLoud::Wav wave;
    };

	/**
	* Class returned by AudioEngine::PlaySound
	* Allows to modify sound properties while it's playing
	*/
    class CATWARE_API AudioHandle
    {
    public:
		AudioHandle( Sound* sound, int soloudHandle );

		virtual void Update( );

        void Pause( ); // This function can be called again to resume
        void Stop( );
    
		void SetLooping( bool looping );
		void SetVolume( float volume ); // Volume goes from 0 - 1
        void SetPan( float pan );
        void SetPitch( float pitch );
		void SetSpeed( float speed );

		inline bool IsLooping( ) { return looping; }
		inline bool IsPasued( ) { return isPaused; }
		inline bool IsFinished( ) { return isFinished; }
		inline float GetVolume( ) { return volume; }
		inline float GetPan( ) { return pan; }
		inline float GetPitch( ) { return pitch; }
		inline float GetSpeed( ) { return speed; }

		Sound* soundResource;

    protected:
		int soloudHandle;

        bool looping = false;
        bool isPaused = false;
        bool isFinished = false;

		float speed = 1.0;
        float volume = 1.0;
        float pan = 0.0;
        float pitch = 1.0;
    };

	class CATWARE_API AudioListener2D
	{
	public:
		AudioListener2D( Vector2D position, float volume );

		Vector2D position;
		float volume;
	};

    /**
	* Basically the same thing as AudioHandle
	* Except it's used for 2D spatial audio
	*/
    class CATWARE_API AudioHandle2D : public AudioHandle
    {
    public:
        AudioHandle2D( Sound* sound, Vector2D position, float volume, double radius, int soloudHandle );

		void Update( ) override;

		Vector2D position;
		float volumeCenter; // Todo: find a beter name for this
		double radius;
    };

	/**
	* Handles almost everything related to sound
	*/
    class CATWARE_API AudioEngine
    {
	public:
		static void InitAudio( );
		static void DeInitAudio( );

        static AudioHandle* PlaySound( Sound* sound );
    	static AudioHandle2D* PlaySound2D( Sound* sound, Vector2D position, float volume, double radius );

		static void UpdateHandles( );

		inline static SoLoud::Soloud* GetSoLoudInstance() { return &soloud; }

		static AudioListener2D audioListener;
	private:
		static std::vector<AudioHandle*> audioHandles;
		static SoLoud::Soloud soloud;
    };
}