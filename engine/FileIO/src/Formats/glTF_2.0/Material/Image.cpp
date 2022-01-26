
#include "Image.h"

#include <atomic>
#include <string>
#include "SOIL.h"
#include "engine/FileIO/src/FileStatus.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	Image::Image() :
		bufferViewIndex(-1),
		imageData(nullptr),
		width(0),
		height(0),
		format(0)
	{

	}

	Image::~Image()
	{
		SOIL_free_image_data((unsigned char*)imageData);
	}

	void Image::fromGLTF(JSON* imageJSON, std::string JSONPath)
	{
		name = imageJSON->getFieldString("name");

		JSON* imageURIJSON = imageJSON->getField("uri");
		if (imageURIJSON != nullptr)
		{
			std::string uri = imageJSON->getFieldString("uri");
			if (fs::path(uri).is_absolute() == false)
			{
				uri = fs::path(JSONPath).parent_path().string() + "\\" + uri;
			}
			loadImage(uri, &imageData, &width, &height, &format);
		}
		else
		{
			std::string mimeType = imageJSON->getFieldString("mimeType");
			JSON* numberJSON = imageJSON->getField("bufferView");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					bufferViewIndex = (int32_t)numberJSON->getNumber();
				}
			}
		}
	}

	void loadImage(std::string filename, void** imageData, int* width, int* height, int* format)
	{
		*imageData = nullptr;
		*imageData = SOIL_load_image(filename.c_str(), width, height, 0, SOIL_LOAD_RGBA);
		*format = SOIL_LOAD_RGBA;
		return;
	}

	void loadImage(std::string filename, void** image, std::atomic<FileStatus>* flag)
	{
		flag->store(FileStatus::LOADING);
		*image = nullptr;
		Image* tmpImage = new Image();
		loadImage(filename, &tmpImage->imageData, &(tmpImage->width), &(tmpImage->height), &(tmpImage->format));
		*image = (void*)tmpImage;
		flag->store(FileStatus::READY);
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