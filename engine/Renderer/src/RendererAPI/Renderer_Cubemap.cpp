
#include "FileIO/src/FileIO_Headers.h"
#include <filesystem>
namespace fs = std::filesystem;

#include "Renderer_Skybox.h"

namespace render
{
	void loadCubemap(std::string dirName, void** cubemap, char* flag)
	{
		*cubemap = nullptr;
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
			char texType[3];
			texType[2] = '\0';
			std::string tmpPath = de.path().filename().replace_extension("").string();
			texType[1] = std::tolower(tmpPath[tmpPath.length() - 1]);
			texType[0] = std::tolower(tmpPath[tmpPath.length() - 2]);

			if (texType[0] == 'f' && texType[1] == 't')
			{
				frontPath = de.path().string();
			}
			else if (texType[0] == 'b' && texType[1] == 'k')
			{
				backPath = de.path().string();
			}
			else if (texType[0] == 'u' && texType[1] == 'p')
			{
				upPath = de.path().string();
			}
			else if (texType[0] == 'd' && texType[1] == 'n')
			{
				downPath = de.path().string();
			}
			else if (texType[0] == 'r' && texType[1] == 't')
			{
				rightPath = de.path().string();
			}
			else if (texType[0] == 'l' && texType[1] == 'f')
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
			Cubemap* tmpCubemap = new Cubemap();

			fileio::Image* front;
			fileio::Image* back;
			fileio::Image* up;
			fileio::Image* down;
			fileio::Image* right;
			fileio::Image* left;

			char flags[6];
			fileio::loadImage(frontPath, (void**)&front, flags);
			fileio::loadImage(backPath, (void**)&back, flags + 1);
			fileio::loadImage(upPath, (void**)&up, flags + 2);
			fileio::loadImage(downPath, (void**)&down, flags + 3);
			fileio::loadImage(rightPath, (void**)&right, flags + 4);
			fileio::loadImage(leftPath, (void**)&left, flags + 5);

			tmpCubemap->textures[0] = *right;
			tmpCubemap->textures[1] = *left;
			tmpCubemap->textures[2] = *up;
			tmpCubemap->textures[3] = *down;
			tmpCubemap->textures[4] = *back;
			tmpCubemap->textures[5] = *front;

			delete front;
			delete back;
			delete up;
			delete down;
			delete right;
			delete left;

			(*cubemap) = (void*)tmpCubemap;

			*flag = 1;
		}
	}
}