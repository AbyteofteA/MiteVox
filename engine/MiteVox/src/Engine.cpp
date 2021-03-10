
#include "Engine.h"

#include "EngineSettings.h"
#include "Scene.h"
#include "engine/UIEventHandler/src/InputHandler.h"

#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Engine::Engine(int argc, char* argv[])
	{
		mathem::Octree<float>* octree = new mathem::Octree<float>(50, 10);

		std::string executionDir = fs::path(argv[0]).parent_path().string();
		settings = new EngineSettings(executionDir);

		inputHandler = new InputHandler(settings->renderer->getWindow());
		inputHandler->attach();
		inputHandler->update();

		onCreate();
	}

	Engine::~Engine()
	{
		onDestroy();
		
		for (auto scene : scenes)
		{
			delete scene.second;
		}
		delete inputHandler;

		delete settings;
	}

	int Engine::createScene(std::string name)
	{
		int ID = IDGenerator.getID();
		scenes.insert( { ID, new Scene() } );
		scenes[ID]->name = name;
		scenes[ID]->renderer = settings->renderer;
		scenes[ID]->inputHandler = inputHandler;

		return ID;
	}

	void Engine::deleteScene(int ID)
	{
		delete scenes[ID];
		scenes.erase(ID);
	}

	Scene* Engine::getActiveScene()
	{
		return scenes[activeScene];
	}

	void Engine::update()
	{
		onUpdate();

		inputHandler->update();
		if (scenes.count(activeScene) > 0)
		{
			scenes[activeScene]->update(settings);
		}
	}
}
