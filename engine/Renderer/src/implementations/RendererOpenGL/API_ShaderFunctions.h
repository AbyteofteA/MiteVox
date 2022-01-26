
#ifndef SHADERFUNCTIONS_H
#define SHADERFUNCTIONS_H

#include "engine/FileIO/src/FileLoaderAsync.h"
#include <string>

namespace render
{
	inline int createShader(std::string name, std::string filename)
	{
		for (int i = 0; i < MAX_SHADERS; i++)
		{
			if (shaders[i] == nullptr)
			{
                char* vertexSource = nullptr;
                char* fragmentSource = nullptr;

                std::string vertPath = filename + ".vert.shader";
                std::string fragPath = filename + ".frag.shader";

                fileio::fileLoader.loadAndParseAsync(vertPath, (void**)&vertexSource);
                fileio::fileLoader.loadAndParseAsync(fragPath, (void**)&fragmentSource);
                fileio::fileLoader.awaitAll();

				shaders[i] = new ShaderOpenGL(vertexSource, fragmentSource, name);
				
				return i;
			}
		}

		return -1;
	}

	inline void useShader(int shaderID)
	{
		shaders[shaderID]->use();
	}

	inline void deleteShader(int shaderID)
	{
		delete shaders[shaderID];
		shaders[shaderID] = nullptr;
	}
}

#endif