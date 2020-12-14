
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	class MiteVox_Engine
	{
	public:
		std::string executionPath = "";

		std::vector<MiteVox_Scene*> scenes;
		long activeScene = -1;

		InputHandler* inputHandler = nullptr;
		render::RendererSettings* renderer = nullptr;

		MiteVox_Engine(int argc, char* argv[])
		{
			init(argc, argv);
			onCreate();
		}
		~MiteVox_Engine()
		{
			onDestroy();
			wipe();
		}

		void init(int argc, char* argv[])
		{
			fs::path currentPath = fs::path(argv[0]);
			executionPath = currentPath.parent_path().string();

			renderer = new render::RendererSettings();
			render::initRenderer(renderer);

			inputHandler = new InputHandler(renderer->getWindow());
			inputHandler->attach();
			inputHandler->update();
		}
		void wipe()
		{
			for (long i = 0; i < (long)scenes.size(); i++)
			{
				scenes[i]->wipe();
			}
			delete inputHandler;
			render::closeRenderer(renderer);
		}

		void onCreate();
		void onUpdate();
		void onDestroy();
		
		long createScene(std::string name = "Untitled")
		{
			long index = scenes.size();
			scenes.push_back(new MiteVox_Scene());
			scenes[index]->name = name;
			scenes[index]->renderer = renderer;
			scenes[index]->inputHandler = inputHandler;

			return index;
		}
		void deleteScene(long index)
		{
			scenes[index]->wipe();
		}

		MiteVox_Scene* getActiveScene()
		{
			return scenes[activeScene];
		}

		void update()
		{
			onUpdate();

			inputHandler->update();
			if ((activeScene > -1) && (activeScene < (long)scenes.size()))
			{
				scenes[activeScene]->update();
			}
		}
	};
}

#endif