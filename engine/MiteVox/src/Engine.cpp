#include "Engine.h"

#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Playground.h"
#include "engine/MiteVox/src/Rendering/drawCollider.h"
#include "engine/MiteVox/src/Rendering/drawCollisions.h"
#include "engine/MiteVox/src/Rendering/drawSceneOctree.h"
#include "engine/MiteVox/src/Rendering/drawAxes.h"
#include "engine/MiteVox/src/Rendering/drawLightSource.h"
#include "engine/MiteVox/src/Rendering/Lighting/collectLights.h"
#include "engine/MiteVox/src/Physics/computePhysics.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"

#include "engine/FileIO/src/Formats/CodecGLTF/CodecGLTF.h"
#include "engine/MiteVox/src/Animation/MorphTargetAnimation/applyMorphTargetAnimation.h"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Engine::Engine(int argc, char* argv[]) : 
		dataPointsContainers(32, 8) // TODO: add to EngineSettings
	{
		MiteVoxAPI::init(this);

		prevCycleTime = std::chrono::steady_clock::now();

		std::string executionDir = fs::absolute(fs::path(argv[0])).parent_path().string();
		settings = new EngineSettings(executionDir);

		entitiesAllocator.allocate(256); // TODO: add to EngineSettings

		pointLightsArray.reserve(64); // TODO: add to EngineSettings
		directionalLightsArray.reserve(64); // TODO: add to EngineSettings
		spotLightsArray.reserve(64); // TODO: add to EngineSettings
		entitiesToSimulate.reserve(64); // TODO: add to EngineSettings
		collisions.reserve(64); // TODO: add to EngineSettings

		// TODO: add to EngineSettings

		playground = new Playground();
		if (argc > 1)
		{
			fileio::CodecGLTF* playgroundGLTF = new fileio::CodecGLTF(playground);
			playgroundGLTF->readFromFile(std::string(argv[1]));
			// TODO: add checks
		}
		else
		{
			playground->createDefaultScene(settings);
		}

		preparePlayground();

		// Compile shaders.

		std::string shadersDir = settings->getResourceDir() + "/shaders";
		basicShader = render::createShader("Basic Shader", shadersDir + "/basic/basic");
		skyboxShader = render::createShader("Skybox Shader", shadersDir + "/skybox/skybox");
		primitiveShader = render::createShader("Primitive Shader", shadersDir + "/primitive/primitive");
		settings->getRendererSettings()->primitiveShaderID = primitiveShader;

		uploadScene(playground->scenes.getElement(0), basicShader);

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
			deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - prevCycleTime).count();
			prevCycleTime = now;

			// Skip large time steps (TODO: add to EngineSettings)
			if (deltaTime > 1.0f)
			{
				continue;
			}

			onUpdate();

			InputHandler* inputHandler = InputHandler::getInstance();
			inputHandler->update();
			
			Scene* scene = playground->getActiveScene();
			if (scene == nullptr)
			{
				continue;
			}

			scene->update(deltaTime); // Scripts are executed here

			// Space culling
			MiteVoxAPI::collectEntitiesToSimulate();

			// Cleanup
			scene->timeSinceCleanup += deltaTime;
			if (scene->timeSinceCleanup > settings->getCleanupPeriod())
			{
				scene->timeSinceCleanup = 0.0f;
				std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
				// TODO: Implement cleanup.
			}

			// Physics and Transform
			collisions.clear();
			dataPointsContainers.returnAllContainers();
			size_t substepsCount = MiteVoxAPI::getSubstepsCount();
			float substepDeltaTime = deltaTime / (float)substepsCount;
			for (size_t i = 0; i < substepsCount; ++i)
			{
				applyDampingAndSleeping(&entitiesToSimulate, substepDeltaTime);
				computeIntegration(&entitiesToSimulate, substepDeltaTime);
				computePhysics(MiteVoxAPI::getPhysicsSolverType(), substepDeltaTime, settings->equalityTolerance);
			}

			// Reset forces and torques
			size_t entitiesToSimulateCount = entitiesToSimulate.getElementsCount();
			for (size_t i = 0; i < entitiesToSimulateCount; ++i)
			{
				Entity* entity = entitiesToSimulate.getElement(i);
				entity->resetForces();
			}

			animateNodes(&entitiesToSimulate, deltaTime);

			// Renderer
			scene->timeSinceRendererUpdate += deltaTime;
			if (scene->timeSinceRendererUpdate > settings->getRendererPeriod())
			{
				render::RendererSettings* renderer = settings->getRendererSettings();
				scene->timeSinceRendererUpdate = 0.0f;
				renderer->amountOfDrawCalls = 0;

				pointLightsArray.clear();
				directionalLightsArray.clear();
				spotLightsArray.clear();

				Entity* activeCameraEntity = MiteVoxAPI::getActiveCameraEntity();
				render::Camera* camera = nullptr;
				mathem::GeometryTransform cameraTransform = activeCameraEntity->getCamera(&camera);

				render::clearBufferXY(renderer->clearColor);
				render::clearBufferZ();

				if (scene->activeSkybox >= 0)
				{
					render::Cubemap* skybox = &scene->skyboxes.at(scene->activeSkybox);
					render::renderSkybox(renderer, skyboxShader, skybox, camera, &cameraTransform);
				}

				renderEntities(deltaTime, renderer, scene, camera, &cameraTransform);

				if (settings->debug)
				{
					drawAxes(renderer);

					if (scene->foundation != nullptr)
					{
						drawSceneOctree(renderer, scene->foundation->octree);
					}

					drawCollisions(renderer, &collisions);
				}

				// Render primitives.
				render::renderPoints(renderer, camera, &cameraTransform);
				render::renderLines(renderer, camera, &cameraTransform);
				render::renderTriangles(renderer, camera, &cameraTransform);

				render::display(renderer);
			}

			//std::cout << "Amount of draw calls: " << settings->getRendererSettings()->amountOfDrawCalls << std::endl;
		}

		if (playground)
		{
			fileio::CodecGLTF* playgroundGLTF = new fileio::CodecGLTF(playground);
			playgroundGLTF->saveToFile(settings->executionDir + "\\playground.gltf");
		}
	}

	Entity* Engine::useEntity()
	{
		return entitiesAllocator.useElement();
	}

	void Engine::returnEntity(Entity* usedEntity)
	{
		entitiesAllocator.returnUsedElement(usedEntity);
	}

	void Engine::preparePlayground()
	{
		// TODO: create load/unload system
		if (Scene* activeScene = playground->getActiveScene())
		{
			size_t activeEntitiesCount = activeScene->entities.getElementsCount();
			for (size_t i = 0; i < activeEntitiesCount; ++i)
			{
				Entity* entity = activeScene->entities.getElement(i);
				entity->tryGenerateHitbox();
				activeScene->foundation->emplace(entity); // TODO: delete
			}
		}

		// Add a default camera to every scene that doesn't have one
		if (1/*playground->cameras.getElementsCount() == 0*/)
		{
			MiteVoxAPI::createFPSCharacter("FPS Character");
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

	void Engine::animateNodes(safety::SafeArray<Entity*>* entities, float deltaTime)
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

		size_t entitiesCount = entities->getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			animateNodeRecursively(entities->getElement(i)->renderableNode, deltaTime);
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

	void Engine::uploadScene(Scene* scene, int shaderID)
	{
		if (scene == nullptr)
		{
			return;
		}

		size_t entitiesCount = scene->entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			uploadNodeRecursively(scene->entities.getElement(i)->renderableNode, shaderID);
		}
	}

	void Engine::renderEntities(
		float deltaTime,
		render::RendererSettings* renderer,
		Scene* scene,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform)
	{
		collectLights(&entitiesToSimulate, &pointLightsArray, &directionalLightsArray, &spotLightsArray);

		render::setAmbientLight(scene->ambientLight, basicShader);
		render::uploadDirectionalLights(&directionalLightsArray, basicShader);
		render::uploadPointLights(&pointLightsArray, basicShader);
		render::uploadSpotLights(&spotLightsArray, basicShader);

		if (renderer->backfaceCulling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		// Render 3D models.
		size_t entitiesCount = entitiesToSimulate.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = entitiesToSimulate.getElement(i);
			
			if (settings->debug)
			{
				MiteVoxAPI::renderNodeRecursively(
					basicShader,
					entity->renderableNode,
					&entity->transform,
					camera,
					cameraTransform);

				if (entity->isSleeping() == false)
				{
					drawCollider(settings->renderer, entity->getCollider(), &entity->transform);
				}
			}
			else
			{
				MiteVoxAPI::renderNodeRecursively(
					basicShader,
					entity->renderableNode,
					&entity->transform,
					camera,
					cameraTransform);

				// TODO: delete
				if (entity->isSleeping() == false)
				{
					drawCollider(settings->renderer, entity->getCollider(), &entity->transform);
				}
			}
		}
	}
}
