
#include "Scene.h"

#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/entityID.h"

#include "engine/MiteVox/src/callbacks/NativeScript_Callbacks.h"
#include "engine/MiteVox/src/callbacks/Transform_Callbacks.h"

namespace mitevox
{
	Scene::Scene()
	{
		// TODO: add corresponding settings
		foundation = new SceneFoundation(250.0f, 0.1f, 1, 5, 0, 100);

		ECS = new ecs::EntityComponentSystem((entityID)100);

		// Math components

		Transform_Component = ECS->registerComponent("Transform", sizeof(mathem::Transform),
			Transform_onCreate);

		// Script components

		NativeScript_Component = ECS->registerComponent("NativeScript_ECS", sizeof(NativeScript_ECS),
			NativeScript_onCreate, NativeScript_onUpdateAll, NativeScript_onDestroy);
	}

	Scene::Scene(EngineSettings* _settings, entityID initialEntitiesBufferSize) : Scene()
	{
		ECS->reserveEntities(initialEntitiesBufferSize);
		settings = _settings;
	}

	Scene::~Scene()
	{
		delete ECS;
	}

	void Scene::update(float deltaTime)
	{
		currentTime += deltaTime;
	}

	float Scene::getCurrentTime()
	{
		return currentTime;
	}
}
