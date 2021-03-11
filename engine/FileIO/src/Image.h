
#ifndef IMAGE_H
#define IMAGE_H

#include <atomic>
#include <string>
#include "SOIL.h"

namespace fileio
{
	struct Image
	{
	public:

		void* imageData = nullptr;
		int width = 0;
		int height = 0;
		int format = 0;

		inline ~Image()
		{
			SOIL_free_image_data((unsigned char*)imageData);
		}
	};


	inline void loadImage(std::string filename, void** image, std::atomic<FileStatus>* flag)
	{
		*image = nullptr;
		flag->store(FileStatus::LOADING);

		Image* tmpImage = new Image();

		tmpImage->imageData = SOIL_load_image(filename.c_str(), &(tmpImage->width), &(tmpImage->height), 0, SOIL_LOAD_RGBA);
		tmpImage->format = SOIL_LOAD_RGBA;

		*image = (void*)tmpImage;

		flag->store(FileStatus::READY);
		return;
	}
}

#endif