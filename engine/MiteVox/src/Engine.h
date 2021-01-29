
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "EngineSettings.h"

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
		EngineSettings* settings;

		Engine(int argc, char* argv[])
		{
			init(argc, argv);
			onCreate();
		}
		~Engine()
		{
			onDestroy();
			wipe();
			delete settings;
		}

		void init(int argc, char* argv[])
		{
			std::string executionDir = fs::path(argv[0]).parent_path().string();
			settings = new EngineSettings(executionDir);

			inputHandler = new InputHandler(settings->renderer->getWindow());
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
			
		}

		void onCreate();
		void onUpdate();
		void onDestroy();
		
		long createScene(std::string name = "Untitled")
		{
			long index = scenes.size();
			scenes.push_back(new Scene());
			scenes[index]->name = name;
			scenes[index]->renderer = settings->renderer;
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