
#include "Scene.h"

#include "EngineSettings.h"
#include "entityID.h"

#include "callbacks/NativeScript_Callbacks.h"
#include "callbacks/Model3D_Callbacks.h"
#include "callbacks/Transform_Callbacks.h"
#include "callbacks/DirectedLight_Callbacks.h"
#include "callbacks/PointLight_Callbacks.h"
#include "callbacks/SpotLight_Callbacks.h"

namespace mitevox
{
	Scene::Scene()
	{

	}

	Scene::Scene(EngineSettings* _settings, entityID initialEntitiesBufferSize)
	{
		ECS = new ecs::EntityComponentSystem(initialEntitiesBufferSize);
		settings = _settings;

		// Renderer components

		Camera_Component = ECS->registerComponent("Camera", sizeof(render::Camera));

		Model3D_Component = ECS->registerComponent("Model3D", sizeof(render::Model3D),
			Model3D_onCreate, Model3D_onUpdateAll, Model3D_onDestroy);

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

	Scene::~Scene()
	{
		delete ECS;
	}

	double Scene::getCurrentTime()
	{
		return currentTime;
	}
}
