
#include "Playground.h"

#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

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
		size_t scenesCount = scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			delete scenes.getElement(i);
		}
	}

	size_t Playground::createScene(std::string name, EngineSettings* settings)
	{
		auto newScene = new Scene(settings);
		newScene->name = name;
		scenes.appendElement(newScene);
		return scenes.getElementsCount() - 1;
	}

	size_t Playground::createActiveScene(std::string name, EngineSettings* settings)
	{
		size_t index = createScene(name, settings);
		return activeScene = index;
	}

	void Playground::deleteScene(size_t index)
	{
		delete scenes.getElement(index);
		scenes.removeElement(index);
	}

	Scene* Playground::getActiveScene()
	{
		return scenes.getElement(activeScene);
	}
}
