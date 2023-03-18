
#ifndef IMAGECODEC_H
#define IMAGECODEC_H

#include "engine/MiteVox/src/Material/Image.h"
#include "engine/FileIO/src/FileStatus.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include <atomic>
#include <string>

namespace fileio
{
	class ImageCodec
	{
	public:

		static void fromGLTF(mitevox::Image* imageResult, JSON* imageJSON, std::string JSONPath);
		static void toGLTF(JSON* imageJSONResult, mitevox::Image* image, size_t index, std::string JSONPath);
	};
}

#endif