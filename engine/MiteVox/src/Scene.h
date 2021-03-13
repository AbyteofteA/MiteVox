
#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

#include <vector>

#ifndef INITIAL_ENTITY_BUFFER_SIZE
#define INITIAL_ENTITY_BUFFER_SIZE 1024
#endif

// Default components IDs //

#define CAMERA_COMPONENT 0
#define MODEL3D_COMPONENT 1
#define POINTLIGHT_COMPONENT 2
#define DIRECTEDLIGHT_COMPONENT 3
#define SPOTLIGHT_COMPONENT 4
#define TRANSFORM_COMPONENT 5
#define NATIVE_SCRIPT_COMPONENT 6
#define PRIMITIVECOLLIDER_COMPONENT 7
#define RIGIDBODY_COMPONENT 8
#define TAG_COMPONENT 9

#include "EngineSettings.h"
#include "entityID.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"

namespace mitevox
{
	class Scene
	{
	public:

		std::string name = "Untitled";
		//mathem::Octree<float>* octree;
		ecs::EntityComponentSystem<entityID>* ECS = nullptr;
		std::vector<render::Skybox> skyboxes;
		long activeSkybox = -1;
		entityID activeCamera = -1;

		EngineSettings* settings = nullptr;

		//************************************ Components *****************************************

		// Renderer components

		MANAGER_INDEX_TYPE Camera_Component = 0;
		MANAGER_INDEX_TYPE Model3D_Component = 0;
		MANAGER_INDEX_TYPE PointLight_Component = 0;
		MANAGER_INDEX_TYPE DirectedLight_Component = 0;
		MANAGER_INDEX_TYPE SpotLight_Component = 0;

		// Math components

		MANAGER_INDEX_TYPE Transform_Component = 0;

		// Script components

		MANAGER_INDEX_TYPE NativeScript_Component = 0;

		// Physics components

		MANAGER_INDEX_TYPE PrimitiveCollider_Component = 0;
		MANAGER_INDEX_TYPE RigidBody_Component = 0;

		// Other components

		MANAGER_INDEX_TYPE Tag_Component = 0;


		Scene(EngineSettings* _settings, entityID initialEntitiesBufferSize = INITIAL_ENTITY_BUFFER_SIZE);
		~Scene();

		void update();

		// Getters //

		double getCurrentTime();

	private:

		std::chrono::high_resolution_clock::time_point prevCycleTime;
		std::chrono::high_resolution_clock::time_point cleanupTime;
		std::chrono::high_resolution_clock::time_point physicsTime;
		std::chrono::high_resolution_clock::time_point rendererTime;
		std::chrono::high_resolution_clock::time_point now;
		double dt = 0.0;
		double currentTime = 0.0;

		// Temporary time storage.
		double tmpTime = 0.0;

		void updateTimers();

	};
}

#endif