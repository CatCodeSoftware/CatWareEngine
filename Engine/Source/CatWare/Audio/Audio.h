#pragma once

#include <string>
#include <vector>

#include <soloud.h>
#include <soloud_wav.h>

#include "CatWare/Core.h"
#include "CatWare/Utils/Vector.h"

namespace CatWare
{
    class CATWARE_API Sound
    {
	public:
		Sound( std::string soundFilePath );

		inline SoLoud::Wav* GetWave( ) { return &wave; }
	private:
		SoLoud::Wav wave;
    };

    class CATWARE_API AudioHandle
    {
    public:
		AudioHandle( Sound* sound, int soloudHandle );

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

    protected:
		Sound* soundResource;
		int soloudHandle;

        bool looping = false;
        bool isPaused = false;
        bool isFinished = false;

		float speed = 1.0;
        float volume = 1.0;
        float pan = 0.0;
        float pitch = 1.0;
    };

    // 2D Spatial audio
    class CATWARE_API AudioHandle2D : public AudioHandle
    {
    public:
        AudioHandle2D( Sound* sound, Vector2D position, int soloudHandle );

        void SetPosition( );
    };

    class CATWARE_API AudioEngine
    {
	public:
		static void InitAudio( );
		static void DeInitAudio( );

        static AudioHandle* PlaySound( Sound* sound );
    	static AudioHandle2D* PlaySound2D( Sound* sound );

		inline static SoLoud::Soloud* GetSoLoudInstance() { return &soloud; }
	private:
		static std::vector<AudioHandle*> audioHandles;
		static SoLoud::Soloud soloud;
    };
}