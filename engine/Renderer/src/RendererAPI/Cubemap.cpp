#include "Cubemap.h"

#include "engine/FileIO/src/FileLoaderAsync.h"

#include <filesystem>
#include <string>
#include <cctype>
namespace fs = std::filesystem;

namespace render
{
	float skyboxVertices[] =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	Cubemap::Cubemap()
	{

	}
	Cubemap::~Cubemap()
	{

	}

	void Cubemap::deallocate()
	{
		for (int i = 0; i < 6; i++)
		{
			delete& textures[i];
		}
	}

	void loadCubemap(std::string dirName, Cubemap* cubemap, char* flag)
	{
		*flag = 0;

		std::string frontPath;
		std::string backPath;
		std::string upPath;
		std::string downPath;
		std::string rightPath;
		std::string leftPath;

		fs::path currentPath = fs::path(dirName);

		for (fs::directory_entry de : fs::directory_iterator(currentPath))
		{
			std::string cubemapSideName = de.path().filename().replace_extension("").string();
			cubemapSideName = cubemapSideName.substr(cubemapSideName.length() - 2, 2);
			std::transform(cubemapSideName.begin(), cubemapSideName.end(), cubemapSideName.begin(), [](unsigned char c) { return std::tolower(c); });
			if (cubemapSideName == "ft")
			{
				frontPath = de.path().string();
			}
			else if (cubemapSideName == "bk")
			{
				backPath = de.path().string();
			}
			else if (cubemapSideName == "up")
			{
				upPath = de.path().string();
			}
			else if (cubemapSideName == "dn")
			{
				downPath = de.path().string();
			}
			else if (cubemapSideName == "rt")
			{
				rightPath = de.path().string();
			}
			else if (cubemapSideName == "lf")
			{
				leftPath = de.path().string();
			}
		}

		if (frontPath.empty() || upPath.empty() || rightPath.empty() ||
			backPath.empty() || downPath.empty() || leftPath.empty())
		{
			*flag = -1;
			return;
		}
		else
		{
			mitevox::Image* right = new mitevox::Image();
			mitevox::Image* left = new mitevox::Image();
			mitevox::Image* up = new mitevox::Image();
			mitevox::Image* down = new mitevox::Image();
			mitevox::Image* back = new mitevox::Image();
			mitevox::Image* front = new mitevox::Image();
			fileio::fileLoader.loadAndParseAsync(rightPath, (void**)&right, fileio::FileInputOutput::loadImage);
			fileio::fileLoader.loadAndParseAsync(leftPath, (void**)&left, fileio::FileInputOutput::loadImage);
			fileio::fileLoader.loadAndParseAsync(upPath, (void**)&up, fileio::FileInputOutput::loadImage);
			fileio::fileLoader.loadAndParseAsync(downPath, (void**)&down, fileio::FileInputOutput::loadImage);
			fileio::fileLoader.loadAndParseAsync(backPath, (void**)&back, fileio::FileInputOutput::loadImage);
			fileio::fileLoader.loadAndParseAsync(frontPath, (void**)&front, fileio::FileInputOutput::loadImage);
			fileio::fileLoader.awaitAll();
			cubemap->textures[0] = *right;
			cubemap->textures[1] = *left;
			cubemap->textures[2] = *up;
			cubemap->textures[3] = *down;
			cubemap->textures[4] = *back;
			cubemap->textures[5] = *front;

			*flag = 1;
		}
	}
}