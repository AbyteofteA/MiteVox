#include "Scene.h"

#include "engine/MiteVox/src/EngineSettings.h"

namespace mitevox
{
	Scene::Scene()
	{
		// TODO: add corresponding settings
		timeSinceCleanup = 0.0f;
		timeSincePhysicsUpdate = 0.0f;
		timeSinceAnimationsUpdate = 0.0f;
		timeSinceRendererUpdate = 0.0f;
		currentTime = 0.0f;
	}

	Scene::Scene(EngineSettings* _settings) : Scene()
	{
		settings = _settings;
	}

	Scene::~Scene()
	{

	}

	void Scene::update(float deltaTime)
	{
		currentTime += deltaTime;

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
