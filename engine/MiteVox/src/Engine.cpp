
#include "Engine.h"

#include "EngineSettings.h"
#include "Playground.h"

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

		uploadNodesRecursively(&playground->nodes, basicShader);

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
				scene->update(deltaTime);

				// Execute scripts
				scene->ECS->updateComponent(scene->NativeScript_Component, scene);

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
				
				// Renderer
				scene->timeSinceRendererUpdate += deltaTime;
				if (scene->timeSinceRendererUpdate > settings->getRendererPeriod())
				{
					scene->timeSinceRendererUpdate = 0.0f;

					render::RendererSettings* renderer = settings->getRendererSettings();
					renderer->amountOfDrawCalls = 0;

					// Update lights.
					scene->ECS->updateComponent(scene->DirectedLight_Component, scene);
					scene->ECS->updateComponent(scene->PointLight_Component, scene);
					scene->ECS->updateComponent(scene->SpotLight_Component, scene);

					render::Camera* camera =
						(render::Camera*)scene->ECS->getComponent(scene->activeCamera, scene->Camera_Component);
					mathem::Transform* cameraTransform =
						(mathem::Transform*)scene->ECS->getComponent(scene->activeCamera, scene->Transform_Component);

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
					renderNodesRecursively(nodesToRender, basicShader, camera, cameraTransform);

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

	void Engine::prepareNodeRecursively(Node* node)
	{
		if (node->mesh)
		{
			if (node->isAnimatedMesh())
			{
				node->meshAnimationTarget = new Mesh();
				node->mesh->makeCopyForAnimationTo(node->meshAnimationTarget);
			}

			// Apply default material where needed
			size_t meshPrimitivesCount = node->mesh->primitives.getElementsCount();
			for (size_t i = 0; i < meshPrimitivesCount; ++i)
			{
				MeshPrimitive* meshPrimitive = node->mesh->primitives.getElement(i);
				if (meshPrimitive->material == nullptr)
				{
					meshPrimitive->material = new Material();
				}
			}
		}
		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			prepareNodeRecursively(node->children.getElement(i));
		}
	}

	void Engine::preparePlayground()
	{
		size_t nodesCount = playground->nodes.getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			prepareNodeRecursively(playground->nodes.getElement(i));
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

		if (0/*node->skinIndex*/)
		{
			// TODO: apply skeletal animation to node->resultMesh
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

	void Engine::uploadNodesRecursively(safety::SafeArray<Node*>* nodes, int shaderID)
	{
		size_t nodesCount = nodes->getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			render::uploadNodeRecursively(nodes->getElement(i), shaderID);
		}
	}

	void Engine::renderNodesRecursively(
		safety::SafeArray<Node*>* nodes, 
		int shaderID, 
		render::Camera* camera, 
		mathem::Transform* cameraTransform)
	{
		size_t nodesCount = nodes->getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mathem::GeometryTransform transform;
			render::renderNodeRecursively(
				settings->renderer, 
				shaderID, 
				nodes->getElement(i), 
				&transform, 
				camera, 
				cameraTransform);
		}
	}
}
