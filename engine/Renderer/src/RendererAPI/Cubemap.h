#ifndef RENDERER_CUBEMAP_H
#define RENDERER_CUBEMAP_H

#include "engine/MiteVox/src/Material/Image.h"

namespace render
{
	enum struct CubemapType
	{

	};

	struct Cubemap
	{
		Cubemap();
		~Cubemap();
		void deallocate();

		CubemapType type;
		int shaderID = 0;
		unsigned int vertexID = 0;
		unsigned int textureID = 0;
		mitevox::Image textures[6];
	};

	void loadCubemap(std::string dirname, Cubemap* cubemap, char* flag);
}

#endif
