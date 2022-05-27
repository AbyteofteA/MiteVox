#include "Texture.h"

namespace mitevox
{
	Texture::Texture()
	{

	}

	Texture::Texture(Image* image)
	{
		this->image = image;
	}

	uint32_t Texture::getWidth()
	{
		return image->width;
	}

	uint32_t Texture::getHeight()
	{
		return image->height;
	}
}