#include "Scene.h"

#include "engine/MiteVox/src/EngineSettings.h"

namespace mitevox
{
	Scene::Scene()
	{
		// TODO: add corresponding settings
		foundation = new SceneFoundation(250.0f, 0.1f, 1, 5, 0, 100);
	}

	Scene::Scene(EngineSettings* _settings) : Scene()
	{
		settings = _settings;
	}

	Scene::~Scene()
	{
		delete foundation;
	}

	void Scene::update(float deltaTime)
	{
		currentTime += deltaTime;
		foundation->update();

		size_t scriptsCount = scripts.getElementsCount();
		for (size_t i = 0; i < scriptsCount; ++i)
		{
			NativeScript script = scripts.getElement(i);
			script(this);
		}
	}

	float Scene::getCurrentTime()
	{
		return currentTime;
	}
}
