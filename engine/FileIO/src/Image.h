
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "SOIL.h"

namespace fileio
{
	struct Image
	{

	public:

		void* imageData = NULL;
		int width = 0;
		int height = 0;
		int format = 0;

		inline ~Image()
		{
			SOIL_free_image_data((unsigned char*)imageData);
		}
	};


	inline void loadImage(std::string filename, void** image, char* flag)
	{
		*image = NULL;
		*flag = 0;

		Image* tmpImage = new Image();

		tmpImage->imageData = SOIL_load_image(filename.c_str(), &(tmpImage->width), &(tmpImage->height), 0, SOIL_LOAD_RGBA);
		tmpImage->format = SOIL_LOAD_RGBA;

		*image = (void*)tmpImage;

		*flag = 1;
		return;
	}
}

#endif