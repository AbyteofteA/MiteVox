#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/Playground/SceneFoundation.h"
#include "engine/MiteVox/src/EngineSettings.h"
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
		SceneFoundation* foundation = nullptr;
		safety::SafeArray<Entity*> entities;

		std::vector<render::Cubemap> skyboxes;
		long activeSkybox = -1;
		Node* activeCameraNode = nullptr;

		safety::SafeArray<NativeScript> scripts;

		EngineSettings* settings = nullptr;

		float timeSinceCleanup = 0.0f;
		float timeSincePhysicsUpdate = 0.0f;
		float timeSinceAnimationsUpdate = 0.0f;
		float timeSinceRendererUpdate = 0.0f;
		float currentTime = 0.0f;

		Scene();
		Scene(EngineSettings* _settings);
		~Scene();

		void update(float deltaTime);

		// Getters //

		float getCurrentTime();
	};
}

#endif