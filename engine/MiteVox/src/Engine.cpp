
#include "Engine.h"

#include "EngineSettings.h"
#include "Playground.h"

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

		playground = new Playground();

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
			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - prevCycleTime).count();
			prevCycleTime = now;

			onUpdate();

			settings->getInputHandler()->update();
			
			size_t activeScene = playground->activeScene;
			if (activeScene < playground->scenes.getElementsCount())
			{
				Scene* scene = playground->scenes.getElement(activeScene);
				scene->currentTime += deltaTime;

				// Execute scripts
				scene->ECS->updateComponent(scene->NativeScript_Component, scene);

				// Cleanup
				scene->timeSinceCleanup += deltaTime;
				if (scene->timeSinceCleanup > settings->getCleanupPeriod())
				{
					scene->timeSinceCleanup = 0.0f;

					// TODO: Implement cleanup.
				}

				// Physics and Transform
				scene->timeSincePhysicsUpdate += deltaTime;
				if (scene->timeSincePhysicsUpdate > settings->getPhysicsPeriod())
				{
					scene->timeSincePhysicsUpdate = 0.0f;

					// TODO: Implement physics update.
				}

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

					// Render 3D models.
					scene->ECS->updateComponent(scene->Model3D_Component, scene);

					// Render primitives.
					render::Camera* camera =
						(render::Camera*)scene->ECS->getComponent(scene->activeCamera, scene->Camera_Component);
					mathem::Transform* cameraTransform =
						(mathem::Transform*)scene->ECS->getComponent(scene->activeCamera, scene->Transform_Component);

					render::renderPoints(renderer, camera, cameraTransform);
					render::renderLines(renderer, camera, cameraTransform);
					render::renderTriangles(renderer, camera, cameraTransform);

					render::display(renderer);
				}

				//std::cout << "Amount of draw calls: " << settings->getRendererSettings()->amountOfDrawCalls << std::endl;
			}
		}
	}
}
