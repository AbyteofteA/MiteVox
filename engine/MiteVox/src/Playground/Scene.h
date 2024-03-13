#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/NativeScript.h"
#include "engine/Renderer/src/RendererAPI/Cubemap.h"

#include <vector>
#include <string>
#include <chrono>

namespace mitevox
{
	class Scene
	{
	public:

		std::string name = "Untitled";
		safety::SafeArray<Entity*> entities;

		mathem::Vector3D ambientLight = { 0.05f, 0.05f, 0.05f };
		std::vector<render::Cubemap> skyboxes;
		bool isGravityGlobal = true;
		mathem::Vector3D globalGravity = { 0.0f, -9.8f, 0.0f };
		long activeSkybox = -1;
		Entity* activeCameraEntity = nullptr;

		safety::SafeArray<NativeScript> scripts;

		float timeSinceCleanup = 0.0f;
		float timeSincePhysicsUpdate = 0.0f;
		float timeSinceAnimationsUpdate = 0.0f;
		float timeSinceRendererUpdate = 0.0f;
		float currentTime = 0.0f;

		Scene();
		~Scene();

		void update(float deltaTime);

		// Getters //

		float getCurrentTime();
	};
}

#endif