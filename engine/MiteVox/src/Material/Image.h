#ifndef IMAGE_H
#define IMAGE_H

#include <string>

namespace mitevox
{
	enum struct ImageFormat
	{
		GREY = 1,
		GREY_ALPHA,
		RED_GREEN_BLUE,
		RED_GREEN_BLUE_ALPHA,
	};

	class Image
	{
	public:

		std::string name;
		std::string filePath;
		std::string mimeType;
		int32_t bufferViewIndex = -1;
		void* imageData = nullptr;
		int width = 0;
		int height = 0;
		ImageFormat format = ImageFormat::RED_GREEN_BLUE;
	};
}

#endif