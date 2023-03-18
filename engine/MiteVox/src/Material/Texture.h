#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine/MiteVox/src/Material/ImageSampler.h"
#include "engine/MiteVox/src/Material/Image.h"

#include <cstdint>
#include <string>

namespace mitevox
{
	class Texture
	{
	public:

		std::string name;
		ImageSampler* sampler;
		Image* image = nullptr;
		uint32_t ID = 0;

		Texture();
		Texture(Image* image);

		uint32_t getWidth();
		uint32_t getHeight();
	};
}

#endif
