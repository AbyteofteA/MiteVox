#ifndef IMAGESAMPLERCODEC_H
#define IMAGESAMPLERCODEC_H

#include "engine/MiteVox/src/Material/ImageSampler.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

#include <string>

namespace fileio
{
	class ImageSamplerCodec
	{
	public:

		static void fromGLTF(mitevox::ImageSampler* imageSamplerResult, JSON* imageSamplerJSON);
	};
}

#endif