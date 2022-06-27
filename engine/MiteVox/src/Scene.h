
#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Node.h"
#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/entityID.h"
#include "engine/Renderer/src/RendererAPI/Skybox.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"
#include <vector>
#include <string>
#include <chrono>

#ifndef INITIAL_ENTITY_BUFFER_SIZE
#define INITIAL_ENTITY_BUFFER_SIZE 1024
#endif

// Default components IDs //

#define CAMERA_COMPONENT 0
#define POINTLIGHT_COMPONENT 1
#define DIRECTEDLIGHT_COMPONENT 2
#define SPOTLIGHT_COMPONENT 3
#define TRANSFORM_COMPONENT 4
#define NATIVE_SCRIPT_COMPONENT 5

namespace mitevox
{
	class Scene
	{
	public:

		std::string name = "Untitled";
		//mathem::Octree<mitevox::Node*>* octree;
		ecs::EntityComponentSystem<entityID>* ECS = nullptr;
		safety::SafeArray<mitevox::Node*> nodes;

		std::vector<render::Skybox> skyboxes;
		long activeSkybox = -1;
		entityID activeCamera = -1;

		EngineSettings* settings = nullptr;

		float timeSinceCleanup = 0.0f;
		float timeSincePhysicsUpdate = 0.0f;
		float timeSinceRendererUpdate = 0.0f;
		float currentTime = 0.0f;

		//************************************ Components *****************************************

		// Renderer components

		MANAGER_INDEX_TYPE Camera_Component = 0;
		MANAGER_INDEX_TYPE PointLight_Component = 0;
		MANAGER_INDEX_TYPE DirectedLight_Component = 0;
		MANAGER_INDEX_TYPE SpotLight_Component = 0;

		// Math components

		MANAGER_INDEX_TYPE Transform_Component = 0;

		// Script components

		MANAGER_INDEX_TYPE NativeScript_Component = 0;

		Scene();
		Scene(EngineSettings* _settings, entityID initialEntitiesBufferSize = INITIAL_ENTITY_BUFFER_SIZE);
		~Scene();

		void update(float deltaTime);

		// Getters //

		float getCurrentTime();
	};
}

#endif