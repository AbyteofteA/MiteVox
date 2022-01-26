#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine/FileIO/src/Formats/glTF_2.0/Material/ImageSampler.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Image.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

#include <cstdint>
#include <string>

namespace fileio
{
	class Texture
	{
	public:

		ImageSampler sampler;
		Image* image = nullptr;
		uint32_t ID = 0;

		Texture();
		Texture(Image* image);

		uint32_t getWidth();
		uint32_t getHeight();
	};
}

#endif
