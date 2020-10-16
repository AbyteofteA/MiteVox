
#ifndef RENDERER_SKYBOX_H
#define RENDERER_SKYBOX_H

#include "Renderer_Cubemap.h"
#include <string>

namespace render
{
	class Skybox
	{

	public:

		Skybox()
		{
			cubemap = new Cubemap();
		}
		~Skybox()
		{
			delete cubemap;
		}

		char type;

		int shaderID = 0;

		unsigned int vertexID;

		Cubemap* cubemap;
		unsigned int textureID;
	};

	void loadSkybox(std::string dirname, void** skybox, char* flag);
}

#endif