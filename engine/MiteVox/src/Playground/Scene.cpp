
#include "Scene.h"

#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/entityID.h"

#include "engine/MiteVox/src/callbacks/NativeScript_Callbacks.h"
#include "engine/MiteVox/src/callbacks/Transform_Callbacks.h"
#include "engine/MiteVox/src/callbacks/DirectedLight_Callbacks.h"
#include "engine/MiteVox/src/callbacks/PointLight_Callbacks.h"
#include "engine/MiteVox/src/callbacks/SpotLight_Callbacks.h"

namespace mitevox
{
	Scene::Scene()
	{
		ECS = new ecs::EntityComponentSystem((entityID)100);

		// Renderer components

		PointLight_Component = ECS->registerComponent("PointLight", sizeof(render::PointLight),
			nullptr, PointLight_onUpdateAll, PointLight_onDelete);

		DirectedLight_Component = ECS->registerComponent("DirectedLight", sizeof(render::DirectedLight),
			nullptr, DirectedLight_onUpdateAll, DirectedLight_onDelete);

		SpotLight_Component = ECS->registerComponent("SpotLight", sizeof(render::SpotLight),
			nullptr, SpotLight_onUpdateAll, SpotLight_onDelete);

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
