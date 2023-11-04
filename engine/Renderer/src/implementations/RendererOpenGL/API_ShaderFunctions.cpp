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
                std::string geomPath = filename + ".geom.shader";

				std::string vertexSource;
				std::string fragmentSource;
				std::string geometrySource;

                fileio::FileInputOutput::loadText(vertPath, &vertexSource);
                fileio::FileInputOutput::loadText(fragPath, &fragmentSource);
				fileio::FileStatus geometrySourceFileStatus = fileio::FileInputOutput::loadText(geomPath, &geometrySource);

				if (geometrySourceFileStatus == fileio::FileStatus::READY)
				{
					shaders[i] = new ShaderOpenGL(name, (char*)vertexSource.c_str(), (char*)fragmentSource.c_str(), (char*)geometrySource.c_str());
				}
				else
				{
					shaders[i] = new ShaderOpenGL(name, (char*)vertexSource.c_str(), (char*)fragmentSource.c_str());
				}
				
				return i;
			}
		}

		return -1;
	}

	int createComputeShader(std::string name, std::string filename)
	{
		for (int i = 0; i < MAX_SHADERS; i++)
		{
			if (shaders[i] == nullptr)
			{
				std::string computeSource;

				fileio::FileInputOutput::loadText(filename, &computeSource);
				shaders[i] = new ShaderOpenGL(name, (char*)computeSource.c_str());

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