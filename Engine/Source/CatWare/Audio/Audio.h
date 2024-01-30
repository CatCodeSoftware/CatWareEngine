#pragma once

#include <miniaudio.h>

#include "CatWare/Types/Types.h"

namespace CatWare
{
	class AudioEngine
	{
	public:
		static void Init( );

		static void DataCallback( ma_device* device, void* output, const void* input, UInt32 frameCount );

	private:
		inline static ma_device audioDevice;
	};
}
