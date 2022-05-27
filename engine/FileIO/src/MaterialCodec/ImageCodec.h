
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
	};

	void loadImage(std::string filename, void** imageData, int* width, int* height, int* format);
	void loadImage(std::string filename, void** image, std::atomic<FileStatus>* flag);
}

#endif