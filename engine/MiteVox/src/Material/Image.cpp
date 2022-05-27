
#include "Image.h"

#include <atomic>
#include <string>
#include "SOIL.h"
#include "engine/FileIO/src/FileStatus.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Image::~Image()
	{
		SOIL_free_image_data((unsigned char*)imageData);
	}

	void loadImage(std::string filename, void** imageData, int* width, int* height, int* format)
	{
		*imageData = nullptr;
		*imageData = SOIL_load_image(filename.c_str(), width, height, 0, SOIL_LOAD_RGBA);
		*format = SOIL_LOAD_RGBA;
		return;
	}

	void loadImage(std::string filename, void** image, std::atomic<fileio::FileStatus>* flag)
	{
		flag->store(fileio::FileStatus::LOADING);
		*image = nullptr;
		Image* tmpImage = new Image();
		loadImage(filename, &tmpImage->imageData, &(tmpImage->width), &(tmpImage->height), &(tmpImage->format));
		*image = (void*)tmpImage;
		flag->store(fileio::FileStatus::READY);
		return;
	}
}
/*void fileio::loadImage(std::string filename, void** image, char* flag)
{
	*image = nullptr;
	*flag = 0;

	Image* tmpImage = new Image();

	tmpImage->imageData = SOIL_load_image(filename.c_str(), &(tmpImage->width), &(tmpImage->height), 0, SOIL_LOAD_RGBA);
	tmpImage->format = SOIL_LOAD_RGBA;

	*image = (void*)tmpImage;

	*flag = 1;
	return;
}*/