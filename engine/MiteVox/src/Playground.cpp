
#include "Playground.h"

#include "EngineSettings.h"
#include "Scene.h"
#include "engine/UIEventHandler/src/InputHandler.h"

#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Playground::Playground()
	{

	}

	Playground::~Playground()
	{
		for (auto scene : scenes)
		{
			delete scene.second;
		}
	}

	unsigned int Playground::createScene(std::string name)
	{
		unsigned int ID = IDGenerator.getID();
		scenes.insert({ ID, new Scene() });
		scenes[ID]->name = name;
		//scenes[ID]->renderer = settings->renderer;
		//scenes[ID]->inputHandler = settings->inputHandler;

		return ID;
	}

	void Playground::deleteScene(unsigned int ID)
	{
		delete scenes[ID];
		scenes.erase(ID);
	}

	Scene* Playground::getActiveScene()
	{
		return scenes[activeScene];
	}
}
