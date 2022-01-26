
#ifndef RENDERER_CUBEMAP_H
#define RENDERER_CUBEMAP_H

#include "engine/FileIO/src/Formats/glTF_2.0/Material/Image.h"

namespace render
{

	struct Cubemap
	{
		Cubemap()
		{

		}
		~Cubemap()
		{
			for (int i = 0; i < 6; i++)
			{
				delete &textures[i];
			}
		}

		fileio::Image textures[6];
		unsigned int textureID;
	};

	void loadCubemap(std::string dirname, void** cubemap, char* flag);
}

#endif
