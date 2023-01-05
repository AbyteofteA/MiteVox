#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/SceneFoundation.h"
#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/NativeScript.h"
#include "engine/Renderer/src/RendererAPI/Skybox.h"
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
		safety::SafeArray<mitevox::Node*> nodes;

		std::vector<render::Skybox> skyboxes;
		long activeSkybox = -1;
		mitevox::Node* activeCameraNode = nullptr;

		safety::SafeArray<mitevox::NativeScript> scripts;

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