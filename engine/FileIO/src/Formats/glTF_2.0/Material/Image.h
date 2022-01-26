
#ifndef IMAGE_H
#define IMAGE_H

#include <atomic>
#include <string>
#include "engine/FileIO/src/FileStatus.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class Image
	{
	public:

		std::string name;
		std::string mimeType;
		int32_t bufferViewIndex;
		void* imageData;
		int width;
		int height;
		int format;

		Image();
		~Image();
		void fromGLTF(JSON* imageJSON, std::string JSONPath);
	};

	void loadImage(std::string filename, void** imageData, int* width, int* height, int* format);
	void loadImage(std::string filename, void** image, std::atomic<FileStatus>* flag);
}

#endif