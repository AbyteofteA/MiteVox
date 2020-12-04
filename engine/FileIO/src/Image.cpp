/*
#include "SOIL.h"

#include "Image.h"

fileio::Image::~Image()
{
	SOIL_free_image_data((unsigned char*)imageData);
}

void fileio::loadImage(std::string filename, void** image, char* flag)
{
	*image = NULL;
	*flag = 0;

	Image* tmpImage = new Image();

	tmpImage->imageData = SOIL_load_image(filename.c_str(), &(tmpImage->width), &(tmpImage->height), 0, SOIL_LOAD_RGBA);
	tmpImage->format = SOIL_LOAD_RGBA;

	*image = (void*)tmpImage;

	*flag = 1;
	return;
}*/