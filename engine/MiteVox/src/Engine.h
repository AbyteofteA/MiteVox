#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "EngineSettings.h"
#include "engine/MiteVox/src/Playground/Playground.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/Math/src/DataStructures/PileAllocator.h"
#include "engine/Math/src/DataStructures/PileOfContainers.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

#include <chrono>

namespace mitevox
{
	/// <summary>
	/// TODO: create default engine_config.json if it doesn't exist
	/// TODO: add scripting in Native (C++), Lua
	/// TODO: implement Perlin noise
	/// TODO: implement PBR shader
	/// </summary>
	class Engine
	{
		friend class MiteVoxAPI;

	public:

		EngineSettings* settings;

		Engine(int argc, char* argv[]);
		~Engine();

		void onCreate();
		void onUpdate();
		void onDestroy();

		void initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, render::ColorRGBf clearColor);
		void run();

	private:

		std::chrono::steady_clock::time_point prevCycleTime;

		Playground* playground;

		int basicShader = -1;
		int primitiveShader = -1;
		int skyboxShader = -1;

		mathem::PileAllocator<Entity> entitiesAllocator;

		// Temporary containers
		safety::SafeArray<render::PointLight> pointLightsArray;
		safety::SafeArray<render::DirectionalLight> directionalLightsArray;
		safety::SafeArray<render::SpotLight> spotLightsArray;
		safety::SafeArray<Entity*> entitiesToSimulate;
		safety::SafeArray<mathem::CollisionInfo<Entity*>> collisions;
		mathem::PileOfSafeArrays<Entity*> dataPointsContainers;

		Entity* useEntity();
		void returnEntity(Entity* usedEntity);

		void preparePlayground();

		void animateNodeRecursively(Node* node, float deltaTime);
		void animateNodes(safety::SafeArray<Entity*>* entities, float deltaTime);

		void uploadNodeRecursively(Node* node, int shaderID);
		void uploadScene(Scene* scene, int shaderID);
		
		void renderNodeRecursively(
			render::RendererSettings* renderer,
			int shaderID,
			Node* node,
			mathem::GeometryTransform* transform,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform);
		void renderEntities(
			float deltaTime,
			render::RendererSettings* renderer,
			Scene* scene,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform);
	};
}

#endif