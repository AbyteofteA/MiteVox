#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include "engine/FileIO/src/FileInputOutput.h"

#include <string>

namespace render
{
	int createShader(std::string name, std::string filename)
	{
		for (int i = 0; i < MAX_SHADERS; i++)
		{
			if (shaders[i] == nullptr)
			{
                std::string vertPath = filename + ".vert.shader";
                std::string fragPath = filename + ".frag.shader";

				std::string vertexSource;
				std::string fragmentSource;

                fileio::FileInputOutput::loadText(vertPath, &vertexSource);
                fileio::FileInputOutput::loadText(fragPath, &fragmentSource);

				shaders[i] = new ShaderOpenGL((char*)vertexSource.c_str(), (char*)fragmentSource.c_str(), name);
				
				return i;
			}
		}

		return -1;
	}

	void useShader(int shaderID)
	{
		shaders[shaderID]->use();
	}

	void deleteShader(int shaderID)
	{
		delete shaders[shaderID];
		shaders[shaderID] = nullptr;
	}
}