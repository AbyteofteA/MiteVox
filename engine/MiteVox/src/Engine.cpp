
#include "Engine.h"

#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Playground.h"
#include "engine/MiteVox/src/Rendering/drawCollider.h"
#include "engine/MiteVox/src/Rendering/drawSceneOctree.h"
#include "engine/MiteVox/src/Rendering/drawAxes.h"
#include "engine/MiteVox/src/Rendering/Lighting/collectLights.h"

#include "engine/FileIO/src/Formats/CodecGLTF/CodecGLTF.h"
#include "engine/MiteVox/src/Animation/MorphTargetAnimation/applyMorphTargetAnimation.h"

#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Engine::Engine(int argc, char* argv[])
	{
		std::string executionDir = fs::path(argv[0]).parent_path().string();
		settings = new EngineSettings(executionDir);

		if (argc > 1)
		{
			fileio::CodecGLTF* playgroundGLTF = new fileio::CodecGLTF();
			playgroundGLTF->readFromFile(std::string(argv[1]));
			// TODO: add checks
			playground = playgroundGLTF->result;
		}
		else
		{
			playground = new Playground();
		}

		preparePlayground();

		// Compile shaders.

		std::string shadersDir = settings->getResourceDir() + "/shaders";
		basicShader = render::createShader("Basic Shader", shadersDir + "/basic/basic");
		skyboxShader = render::createShader("Skybox Shader", shadersDir + "/skybox/skybox");
		primitiveShader = render::createShader("Primitive Shader", shadersDir + "/primitive/primitive");
		settings->getRendererSettings()->primitiveShaderID = primitiveShader;

		uploadNodes(&playground->nodes, basicShader);

		onCreate();
	}

	Engine::~Engine()
	{
		onDestroy();
		
		if (playground)
		{
			delete playground;
		}
		
		delete settings;
	}

	void Engine::initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, render::ColorRGBf clearColor)
	{

	}

	void Engine::run()
	{
		// TODO: Hide implementation.
		while (!glfwWindowShouldClose(settings->getRendererSettings()->getWindow()))
		{
			GLFWwindow* window = settings->getRendererSettings()->getWindow();
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, true);
			}
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);

			// Update timers
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - prevCycleTime).count();
			prevCycleTime = now;

			onUpdate();

			settings->getInputHandler()->update();
			
			size_t activeScene = playground->activeScene;
			if (activeScene < playground->scenes.getElementsCount())
			{
				Scene* scene = playground->getActiveScene();
				scene->update(deltaTime); // Scripts are executed here

				// Cleanup
				scene->timeSinceCleanup += deltaTime;
				if (scene->timeSinceCleanup > settings->getCleanupPeriod())
				{
					scene->timeSinceCleanup = 0.0f;

					// TODO: Implement cleanup.
				}

				// Space culling
				safety::SafeArray<Node*>* nodesToSimulate;
				safety::SafeArray<Node*>* nodesToRender;
				if (settings->spaceCulling)
				{
					nodesToSimulate = new safety::SafeArray<Node*>();
					nodesToRender = new safety::SafeArray<Node*>();
					// TODO: 
				}
				else
				{
					nodesToSimulate = &scene->nodes;
					nodesToRender = &scene->nodes;
				}

				// Physics and Transform
				scene->timeSincePhysicsUpdate += deltaTime;
				if (scene->timeSincePhysicsUpdate > settings->getPhysicsPeriod())
				{
					scene->timeSincePhysicsUpdate = 0.0f;

					// TODO: Implement physics update.
				}

				animateNodes(nodesToRender, deltaTime);
				// Accumulate deltaTime to work properly.
				/*scene->timeSinceAnimationsUpdate += deltaTime;
				if (scene->timeSinceAnimationsUpdate > settings->getAnimationsPeriod())
				{
					scene->timeSinceAnimationsUpdate = 0.0f;
					animateNodes(nodesToRender, deltaTime);
				}*/
				
				static safety::SafeArray<render::PointLight> pointLightsArray;
				static safety::SafeArray<render::DirectionalLight> directionalLightsArray;
				static safety::SafeArray<render::SpotLight> spotLightsArray;
				pointLightsArray.clear();
				directionalLightsArray.clear();
				spotLightsArray.clear();
				collectPointLights(nodesToRender, &pointLightsArray, &directionalLightsArray, &spotLightsArray);
				
				// Renderer
				scene->timeSinceRendererUpdate += deltaTime;
				if (scene->timeSinceRendererUpdate > settings->getRendererPeriod())
				{
					scene->timeSinceRendererUpdate = 0.0f;

					render::RendererSettings* renderer = settings->getRendererSettings();
					renderer->amountOfDrawCalls = 0;

					// Update lights.
					render::uploadPointLights(&pointLightsArray, basicShader);
					// TODO: render::uploadLights(&LightsArray, basicShader);
					// TODO: render::uploadLights(&LightsArray, basicShader);

					render::Camera* camera = scene->activeCameraNode->camera;
					mathem::GeometryTransform* cameraTransform = scene->activeCameraNode->getTransform();

					render::clearBufferXY(renderer->clearColor);
					render::clearBufferZ();
					if (renderer->backfaceCulling)
					{
						glEnable(GL_CULL_FACE);
					}
					else
					{
						glDisable(GL_CULL_FACE);
					}

					// Render 3D models.
					renderNodes(nodesToRender, basicShader, camera, cameraTransform);

					if (settings->debug)
					{
						drawAxes(this->settings->renderer);

						if (scene->foundation != nullptr)
						{
							drawSceneOctree(this->settings->renderer, scene->foundation->octree);
						}
					}

					// Render primitives.
					render::renderPoints(renderer, camera, cameraTransform);
					render::renderLines(renderer, camera, cameraTransform);
					render::renderTriangles(renderer, camera, cameraTransform);

					render::display(renderer);
				}

				//std::cout << "Amount of draw calls: " << settings->getRendererSettings()->amountOfDrawCalls << std::endl;
			}
		}
	}

	void Engine::preparePlayground()
	{
		// Add a default camera to every scene that doesn't have one
		if (1/*playground->cameras.getElementsCount() == 0*/)
		{
			render::Camera* camera = new render::Camera(50, SCREEN_WIDTH, SCREEN_HEIGHT, 0.001f, 100000);
			Node* cameraNode = new Node();
			cameraNode->camera = camera;
			cameraNode->transform.translation = { 0.0f, 0.0f, 1.0f };
			size_t nodesCount = playground->nodes.getElementsCount();
			playground->cameras.appendElement(camera);
			playground->nodes.appendElement(cameraNode);

			size_t scenesCount = playground->scenes.getElementsCount();
			for (size_t i = 0; i < scenesCount; ++i)
			{
				Scene* scene = playground->scenes.getElement(i);
				scene->activeCameraNode = cameraNode;
			}
		}

		// TODO: create load/unload system
		Scene* activeScene = playground->getActiveScene();
		size_t activeNodesCount = activeScene->nodes.getElementsCount();
		for (size_t i = 0; i < activeNodesCount; ++i)
		{
			Node* node = activeScene->nodes.getElement(i);
			node->tryGenerateHitbox();
			activeScene->foundation->emplace(node);
		}

		size_t scenesCount = playground->scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			playground->scenes.getElement(i)->settings = this->settings;
		}

		size_t animationsCount = playground->animations.getElementsCount();
		for (size_t i = 0; i < animationsCount; ++i)
		{
			playground->animations.getElement(i)->start();
		}
	}

	void Engine::simulateNode(Node* node)
	{

	}

	void Engine::simulateNodes(safety::SafeArray<Node*>* nodes)
	{

	}

	void Engine::animateNodeRecursively(Node* node, float deltaTime)
	{
		if (node->mesh != nullptr && node->mesh->isMorphable())
		{
			applyMorphTargetAnimation(node);
			render::updateMesh(node->getMeshToRender(), basicShader);
		}

		if (node->skeleton)
		{
			// TODO: node->skeleton->globalTransform = ;
			node->skeleton->updateJointMatrices();
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			animateNodeRecursively(node->children.getElement(i), deltaTime);
		}
	}

	void Engine::animateNodes(safety::SafeArray<Node*>* nodes, float deltaTime)
	{
		size_t animationsCount = playground->animations.getElementsCount();
		for (size_t i = 0; i < animationsCount; ++i)
		{
			Animation* animation = playground->animations.getElement(i);
			if (animation->isActive())
			{
				animation->update(deltaTime);
			}
		}

		size_t nodesCount = nodes->getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			animateNodeRecursively(nodes->getElement(i), deltaTime);
		}
	}

	void Engine::uploadNodeRecursively(Node* node, int shaderID)
	{
		if (Mesh* meshToRender = node->getMeshToRender())
		{
			render::uploadMesh(meshToRender, shaderID);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			uploadNodeRecursively(node->children.getElement(i), shaderID);
		}
	}

	void Engine::uploadNodes(safety::SafeArray<Node*>* nodes, int shaderID)
	{
		size_t nodesCount = nodes->getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			uploadNodeRecursively(nodes->getElement(i), shaderID);
		}
	}

	void Engine::renderNodeRecursively(
		render::RendererSettings* renderer,
		int shaderID,
		Node* node,
		mathem::GeometryTransform* transform,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform)
	{
		mathem::GeometryTransform nodeGlobalTransform = *transform * node->transform;

		if (auto meshToRender = node->getMeshToRender())
		{
			render::tryUploadSkeleton(node, shaderID);
			render::renderMesh(renderer, shaderID, meshToRender, &nodeGlobalTransform, camera, cameraTransform);
		}

		if (settings->debug)
		{
			if (node->collider)
			{
				drawCollider(this->settings->renderer, node->collider, &nodeGlobalTransform);
			}
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			renderNodeRecursively(
				renderer,
				shaderID,
				node->children.getElement(i),
				&nodeGlobalTransform,
				camera,
				cameraTransform);
		}
	}

	void Engine::renderNodes(
		safety::SafeArray<Node*>* nodes, 
		int shaderID, 
		render::Camera* camera, 
		mathem::GeometryTransform* cameraTransform)
	{
		size_t nodesCount = nodes->getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mathem::GeometryTransform transform;
			renderNodeRecursively(
				settings->renderer, 
				shaderID, 
				nodes->getElement(i), 
				&transform, 
				camera, 
				cameraTransform);
		}
	}

	void Engine::removeNodeRecursively(Node* node, int shaderID)
	{
		if (Mesh* meshToRender = node->getMeshToRender())
		{
			render::removeMesh(meshToRender, shaderID);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			removeNodeRecursively(node->children.getElement(i), shaderID);
		}
	}
}
