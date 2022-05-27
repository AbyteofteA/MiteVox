
#ifndef IMAGE_H
#define IMAGE_H

#include <atomic>
#include <string>
#include "engine/FileIO/src/FileStatus.h"

namespace mitevox
{
	class Image
	{
	public:

		std::string name;
		std::string mimeType;
		int32_t bufferViewIndex = -1;
		void* imageData = nullptr;
		int width = 0;
		int height = 0;
		int format = 0;

		~Image();
	};

	void loadImage(std::string filename, void** imageData, int* width, int* height, int* format);
	void loadImage(std::string filename, void** image, std::atomic<fileio::FileStatus>* flag);
}

#endif