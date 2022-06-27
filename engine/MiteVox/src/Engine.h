
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "EngineSettings.h"
#include "Playground.h"
#include "engine/Renderer/src/RendererAPI/Color.h"

#include <chrono>
#include <unordered_map>

namespace mitevox
{
	class Engine
	{
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

		void prepareNodeRecursively(Node* node);
		void preparePlayground();

		void simulateNode(Node* node);
		void simulateNodes(safety::SafeArray<Node*>* nodes);

		void animateNodeRecursively(Node* node, float deltaTime);
		void animateNodes(safety::SafeArray<Node*>* nodes, float deltaTime);

		void uploadNodesRecursively(safety::SafeArray<Node*>* nodes, int shaderID);
		void renderNodesRecursively(
			safety::SafeArray<Node*>* nodes,
			int shaderID,
			render::Camera* camera,
			mathem::Transform* cameraTransform);
	};
}

#endif