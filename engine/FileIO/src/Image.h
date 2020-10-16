
#ifndef IMAGE_H
#define IMAGE_H

#include <string>

namespace fileio
{
	struct Image
	{

	public:

		void* imageData = NULL;
		int width = 0;
		int height = 0;
		int format = 0;

		~Image();
	};


	void loadImage(std::string filename, void** image, char* flag);
}

#endif