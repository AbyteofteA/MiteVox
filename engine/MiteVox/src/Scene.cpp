
#include "Scene.h"

#include "EngineSettings.h"

#include "callbacks/NativeScript_Callbacks.h"
#include "callbacks/Model3D_Callbacks.h"
#include "callbacks/Transform_Callbacks.h"
#include "callbacks/DirectedLight_Callbacks.h"
#include "callbacks/PointLight_Callbacks.h"
#include "callbacks/SpotLight_Callbacks.h"

namespace mitevox
{
	Scene::Scene(entityID initialEntitiesBufferSize)
	{
		ECS = new ecs::EntityComponentSystem(initialEntitiesBufferSize);

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

		// Physics components

		PrimitiveCollider_Component = ECS->registerComponent("PrimitiveCollider", sizeof(physcs::PrimitiveCollider));

		RigidBody_Component = ECS->registerComponent("RigidBody", sizeof(physcs::RigidBody));

		// Other components

		Tag_Component = ECS->registerComponent("Tag", sizeof(std::string));

		// Initialize timers.

		prevCycleTime = std::chrono::high_resolution_clock::now();
		now = std::chrono::high_resolution_clock::now();
		cleanupTime = std::chrono::high_resolution_clock::now();
		physicsTime = std::chrono::high_resolution_clock::now();
		rendererTime = std::chrono::high_resolution_clock::now();
	}

	Scene::~Scene()
	{
		delete ECS;
	}

	void Scene::update(EngineSettings* settings)
	{
		now = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - prevCycleTime).count();
		currentTime += dt;
		prevCycleTime = std::chrono::high_resolution_clock::now();

		// Execute scripts
		ECS->updateComponent(NativeScript_Component, (void*)this);

		// Cleanup
		now = std::chrono::high_resolution_clock::now();
		tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - cleanupTime).count();
		if (tmpTime > settings->cleanupPeriod)
		{
			cleanupTime = std::chrono::high_resolution_clock::now();
		}

		// Physics and Transform
		now = std::chrono::high_resolution_clock::now();
		tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - physicsTime).count();
		if (tmpTime > settings->physicsPeriod)
		{
			physicsTime = std::chrono::high_resolution_clock::now();
		}

		// Renderer
		now = std::chrono::high_resolution_clock::now();
		tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - rendererTime).count();
		if (tmpTime > settings->rendererPeriod)
		{
			render::ColorRGBf clearColor = { 0.05f, 0.05f, 0.05f };
			render::clearBufferXY(clearColor);
			render::clearBufferZ();

			// Update lights.
			ECS->updateComponent(DirectedLight_Component, (void*)this);
			ECS->updateComponent(PointLight_Component, (void*)this);
			ECS->updateComponent(SpotLight_Component, (void*)this);

			// Render 3D models.
			ECS->updateComponent(Model3D_Component, &this->activeCamera);

			// Render primitives.
			render::Camera* camera =
				(render::Camera*)ECS->getComponent(this->activeCamera, Camera_Component);
			mathem::Transform* cameraTransform =
				(mathem::Transform*)ECS->getComponent(this->activeCamera, Transform_Component);

			render::renderPoints(renderer, camera, cameraTransform);
			render::renderLines(renderer, camera, cameraTransform);
			render::renderTriangles(renderer, camera, cameraTransform);

			render::display(renderer);
			rendererTime = std::chrono::high_resolution_clock::now();
		}
	}

	double Scene::getCurrentTime()
	{
		return currentTime;
	}
}
