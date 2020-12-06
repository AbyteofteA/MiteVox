
#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

#include <vector>

// Default components indeces
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

#include "callbacks/NativeScript_Callbacks.h"
#include "callbacks/Model3D_Callbacks.h"
#include "callbacks/Transform_Callbacks.h"
#include "callbacks/DirectedLight_Callbacks.h"
#include "callbacks/PointLight_Callbacks.h"
#include "callbacks/SpotLight_Callbacks.h"

namespace mitevox
{
	class MiteVox_Scene
	{
	public:

		std::string name = "Untitled";
		ecs::ECS* ECS = nullptr;
		std::vector<render::Skybox> skyboxes;
		long activeSkybox = -1;
		long activeCamera = -1;

		InputHandler* inputHandler = nullptr;
		render::RendererSettings* renderer = nullptr;

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

		//*************************************** Time ********************************************

		// Default period settings.

		double physicsPeriod = 0.06;
		double rendererPeriod = 0.0;

		// Time points.

		std::chrono::high_resolution_clock::time_point prevCycleTime;
		std::chrono::high_resolution_clock::time_point physicsTime;
		std::chrono::high_resolution_clock::time_point rendererTime;
		std::chrono::high_resolution_clock::time_point now;
		double dt = 0.0;
		double currentTime = 0.0;

		// Temporary time storage.
		double tmpTime = 0.0;

		MiteVox_Scene()
		{
			init();
		}
		~MiteVox_Scene()
		{
			wipe();
		}

		void init()
		{
			ECS = new ecs::ECS();

			// Renderer components

			Camera_Component = ECS->registerComponent<render::Camera>(
				"Camera", doNothing, doNothing, doNothing);
			Model3D_Component = ECS->registerComponent<render::Model3D>(
				"Model3D", Model3D_onCreate, Model3D_onUpdate, Model3D_onDestroy);
			PointLight_Component = ECS->registerComponent<render::PointLight>(
				"PointLight", doNothing, PointLight_onUpdate, PointLight_onDelete);
			DirectedLight_Component = ECS->registerComponent<render::DirectedLight>(
				"DirectedLight", doNothing, DirectedLight_onUpdate, DirectedLight_onDelete);
			SpotLight_Component = ECS->registerComponent<render::SpotLight>(
				"SpotLight", doNothing, SpotLight_onUpdate, SpotLight_onDelete);

			// Math components

			Transform_Component = ECS->registerComponent<mathem::Transform>(
				"Transform", Transform_onCreate, doNothing, doNothing);

			// Script components

			NativeScript_Component = ECS->registerComponent<NativeScript_ECS>(
				"NativeScript_ECS", NativeScript_onCreate, NativeScript_onUpdate, NativeScript_onDestroy);

			// Physics components

			PrimitiveCollider_Component = ECS->registerComponent<physcs::PrimitiveCollider>(
				"PrimitiveCollider", doNothing, doNothing, doNothing);
			RigidBody_Component = ECS->registerComponent<physcs::RigidBody>(
				"RigidBody", doNothing, doNothing, doNothing);

			// Other components

			Tag_Component = ECS->registerComponent<std::string>(
				"Tag", doNothing, doNothing, doNothing);

			// Initialize timers.

			prevCycleTime = std::chrono::high_resolution_clock::now();
			now = std::chrono::high_resolution_clock::now();
			physicsTime = std::chrono::high_resolution_clock::now();
			rendererTime = std::chrono::high_resolution_clock::now();
		}
		void wipe()
		{
			ECS->wipe();
		}

		void update()
		{
			now = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - prevCycleTime).count();
			currentTime += dt;
			prevCycleTime = std::chrono::high_resolution_clock::now();

			// Execute scripts
			ECS->updateComponent(NativeScript_Component, (void*)this);

			// Physics and Transform
			now = std::chrono::high_resolution_clock::now();
			tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - physicsTime).count();
			if (tmpTime > physicsPeriod)
			{
				physicsTime = std::chrono::high_resolution_clock::now();
			}

			// Renderer
			now = std::chrono::high_resolution_clock::now();
			tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - rendererTime).count();
			if (tmpTime > rendererPeriod)
			{
				render::clearBufferXY(0.05f, 0.05f, 0.05f);
				render::clearBufferZ();

				// Update lights.
				ECS->updateComponent(DirectedLight_Component, (void*)this);
				ECS->updateComponent(PointLight_Component, (void*)this);
				ECS->updateComponent(SpotLight_Component, (void*)this);

				// Render 3D models.
				ECS->updateComponent(Model3D_Component, &this->activeCamera);

				// Render primitives.
				render::Camera* camera =
					(render::Camera*)ECS->getComponent(this->activeCamera, CAMERA_COMPONENT);
				mathem::Transform* cameraTransform =
					(mathem::Transform*)ECS->getComponent(this->activeCamera, TRANSFORM_COMPONENT);

				render::renderPoints(renderer, camera, cameraTransform);
				render::renderLines(renderer, camera, cameraTransform);
				render::renderTriangles(renderer, camera, cameraTransform);

				render::display(renderer);
				rendererTime = std::chrono::high_resolution_clock::now();
			}
		}
	};
}

#endif