
#include "Playground.h"

#include "Scene.h"
#include "EngineSettings.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <unordered_map>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Playground::Playground(std::string _name)
	{
		name = _name;
	}

	Playground::~Playground()
	{
		for (auto scene : scenes)
		{
			delete scene.second;
		}
	}

	void Playground::update()
	{
		if (scenes.count(activeScene) > 0)
		{
			scenes[activeScene]->update();
		}
	}

	unsigned int Playground::createScene(std::string name, EngineSettings* settings)
	{
		unsigned int ID = IDGenerator.getID();
		scenes.insert({ ID, new Scene(settings) });
		scenes[ID]->name = name;

		return ID;
	}

	unsigned int Playground::createMainScene(std::string name, EngineSettings* settings)
	{
		unsigned int ID = createScene(name, settings);
		return activeScene = ID;
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
