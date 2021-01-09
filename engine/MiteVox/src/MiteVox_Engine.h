
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "MiteVox_Settings.h"

#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	class Engine
	{

	private:

		InputHandler* inputHandler = nullptr;

		std::vector<Scene*> scenes;

	public:

		long activeScene = -1;
		
		render::RendererSettings* renderer = nullptr;

		EngineSettings settings;

		Engine(int argc, char* argv[])
		{
			init(argc, argv);
			onCreate();
		}
		~Engine()
		{
			onDestroy();
			wipe();
		}

		void init(int argc, char* argv[])
		{
			fs::path currentPath = fs::path(argv[0]);
			settings.executionPath = currentPath.parent_path().string();
			settings.resourcePath = currentPath.parent_path().parent_path().parent_path().parent_path().string();
			settings.resourcePath += "/resources";
			settings.configPath = currentPath.parent_path().parent_path().parent_path().parent_path().string();
			settings.configPath += "/config";

			renderer = new render::RendererSettings();
			render::initRenderer(renderer);

			inputHandler = new InputHandler(renderer->getWindow());
			inputHandler->attach();
			inputHandler->update();
		}
		void wipe()
		{
			for (auto scene : scenes)
			{
				scene->wipe();
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
			scenes.push_back(new Scene());
			scenes[index]->name = name;
			scenes[index]->renderer = renderer;
			scenes[index]->inputHandler = inputHandler;

			return index;
		}
		void deleteScene(long index)
		{
			scenes[index]->wipe();
		}

		Scene* getActiveScene()
		{
			return scenes[activeScene];
		}

		void update()
		{
			onUpdate();

			inputHandler->update();
			if ((activeScene > -1) && (activeScene < (long)scenes.size()))
			{
				scenes[activeScene]->update(settings);
			}
		}
	};
}

#endif