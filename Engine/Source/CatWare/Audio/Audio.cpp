#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace CatWare
{
	void AudioEngine::Init( )
	{
		// 2ma_device_config deviceConfig = ma_device_config_init(  );2
	}

	void AudioEngine::DataCallback( ma_device* device, void* output, const void* input, UInt32 frameCount )
	{
	}
}
