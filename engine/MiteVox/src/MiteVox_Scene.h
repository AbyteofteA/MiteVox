
#ifndef MITEVOX_SCENE_H
#define MITEVOX_SCENE_H

// Default components indeces
#define CAMERA_COMPONENT 0
#define MODEL3D_COMPONENT 1
#define AMBIENTLIGHT_COMPONENT 2
#define POINTLIGHT_COMPONENT 3
#define DIRECTEDLIGHT_COMPONENT 4
#define SPOTLIGHT_COMPONENT 5
#define TRANSFORM_COMPONENT 6
#define NATIVE_SCRIPT_COMPONENT 7
#define PRIMITIVECOLLIDER_COMPONENT 8
#define RIGIDBODY_COMPONENT 9
#define TAG_COMPONENT 10

#include "callbacks/NativeScript_Callbacks.h"
#include "callbacks/Model3D_Callbacks.h"
#include "callbacks/Transform_Callbacks.h"
#include "callbacks/AmbientLight_Callbacks.h"
#include "callbacks/DirectedLight_Callbacks.h"
#include "callbacks/PointLight_Callbacks.h"
#include "callbacks/SpotLight_Callbacks.h"

void processInput_Script(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data);

void rotateCamera_Script(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data);


class MiteVox_Scene
{
public:

	ecs::ECS* main_ECS = new ecs::ECS();

	InputHandler* inputHandler;

	long activeCamera = -1;

	//************************************ Components *****************************************
	// Renderer components
	char Camera_Component = 0;
	char Model3D_Component = 0;
	char AmbientLight_Component = 0;
	char PointLight_Component = 0;
	char DirectedLight_Component = 0;
	char SpotLight_Component = 0;
	// Math components
	char Transform_Component = 0;
	// Script components
	char NativeScript_Component = 0;
	// Physics components
	char PrimitiveCollider_Component = 0;
	char RigidBody_Component = 0;

	char Tag_Component = 0;

	//*************************************** Time ********************************************
	// Default period settings.
	double physicsPeriod = 0.06;
	double rendererPeriod = 0.0;
	// Time points.
	std::chrono::high_resolution_clock::time_point prevCycleTime;
	std::chrono::high_resolution_clock::time_point physicsTime;
	std::chrono::high_resolution_clock::time_point rendererTime;
	std::chrono::high_resolution_clock::time_point now;
	double dt = 0.0;
	double currentTime = 0.0;
	// Temporary time storage.
	double tmpTime = 0.0;

	void (*onSceneCreate)(MiteVox_Scene* scene) = nullptr;
	void (*onSceneUpdate)(MiteVox_Scene* scene) = nullptr;
	void (*onSceneDestroy)(MiteVox_Scene* scene) = nullptr;

	MiteVox_Scene(void (*_onSceneCreate)(MiteVox_Scene* scene), 
				void (*_onSceneUpdate)(MiteVox_Scene* scene),
				void (*_onSceneDestroy)(MiteVox_Scene* scene))
	{
		onSceneCreate = _onSceneCreate;
		onSceneUpdate = _onSceneUpdate;
		onSceneDestroy = _onSceneDestroy;
	}


	void init()
	{

		//********************************* Register components ***********************************
		// Renderer components
		Camera_Component = main_ECS->registerComporent<render::Camera>(
			"Camera", nullptr, nullptr, nullptr);
		Model3D_Component = main_ECS->registerComporent<render::Model3D>(
			"Model3D", Model3D_onCreate, Model3D_onUpdate, Model3D_onDestroy);
		AmbientLight_Component = main_ECS->registerComporent<render::AmbientLight>(
			"AmbientLight", nullptr, AmbientLight_onUpdate, nullptr);
		PointLight_Component = main_ECS->registerComporent<render::PointLight>(
			"PointLight", nullptr, PointLight_onUpdate, PointLight_onDelete);
		DirectedLight_Component = main_ECS->registerComporent<render::DirectedLight>(
			"DirectedLight", nullptr, DirectedLight_onUpdate, DirectedLight_onDelete);
		SpotLight_Component = main_ECS->registerComporent<render::SpotLight>(
			"SpotLight", nullptr, SpotLight_onUpdate, SpotLight_onDelete);
		// Math components
		Transform_Component = main_ECS->registerComporent<mathem::Transform>(
			"Transform", Transform_onCreate, nullptr, nullptr);
		// Script components
		NativeScript_Component = main_ECS->registerComporent<NativeScript_ECS>(
			"NativeScript_ECS", NativeScript_onCreate, NativeScript_onUpdate, NativeScript_onDestroy);
		// Physics components
		PrimitiveCollider_Component = main_ECS->registerComporent<physcs::PrimitiveCollider>(
			"PrimitiveCollider", nullptr, nullptr, nullptr);
		RigidBody_Component = main_ECS->registerComporent<physcs::RigidBody>(
			"RigidBody", nullptr, nullptr, nullptr);

		Tag_Component = main_ECS->registerComporent<std::string>(
			"Tag", nullptr, nullptr, nullptr);


		//****************************** Test subject entity **************************************
		// Create subject.
		unsigned long subject0 = main_ECS->addEntity();
		// Attach Camera to subject.
		render::Camera tmpCamera = { 50, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 100000 };
		main_ECS->attachComponent<render::Camera>(subject0, Camera_Component, &tmpCamera);
		activeCamera = subject0;
		// Attach Transform to subject.
		mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, -700 };
		main_ECS->attachComponent<mathem::Transform>(subject0, Transform_Component, &tmpTransform);
		// Attach script
		NativeScript_ECS tmpNativeScript = { doNothing, processInput_Script, doNothing };
		main_ECS->attachComponent<NativeScript_ECS>(subject0, NativeScript_Component, &tmpNativeScript);
		// Attach PointLight
		/*render::SpotLight tmpSpotLight = { { 0, 0, 0 }, { 0, 1, 0 }, 15,
										{ 1, 1, 1, 1 }, 1, 0.0007, 0.00002 };
		main_ECS->attachComponent<render::SpotLight>(subject0, SpotLight_Component, &tmpSpotLight);*/

		//********************************* Global light ******************************************
		// Create subject.
		/*unsigned long sunLight = main_ECS->addEntity();
		// Attach Camera to subject.
		render::DirectedLight sun = { { 0.5, 1, 0.5 }, { 0.3, 0.2, 0, 1} };
		main_ECS->attachComponent<render::DirectedLight>(sunLight, DirectedLight_Component, &sun);*/


		render::initRenderer();

		// Create inputHandler.
		inputHandler = new InputHandler(render::RendererSettingsDebug.getWindow());
		inputHandler->attach();
		inputHandler->update();

		// Initialize timers.
		prevCycleTime = std::chrono::high_resolution_clock::now();
		now = std::chrono::high_resolution_clock::now();
		physicsTime = std::chrono::high_resolution_clock::now();
		rendererTime = std::chrono::high_resolution_clock::now();

		//onSceneCreate(this);
	}

	void update()
	{
		now = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - prevCycleTime).count();
		currentTime += dt;
		prevCycleTime = std::chrono::high_resolution_clock::now();

		//printf("\n FPS: %4.1f\n", 1 / dt);

		// Process input
		inputHandler->update();

		// Execute scripts
		main_ECS->updateComponent(NativeScript_Component, (void*)this);

		// Physics and Transform
		now = std::chrono::high_resolution_clock::now();
		tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - physicsTime).count();
		if (tmpTime > physicsPeriod)
		{
			
			physicsTime = std::chrono::high_resolution_clock::now();
		}

		// Renderer
		now = std::chrono::high_resolution_clock::now();
		tmpTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - rendererTime).count();
		if (tmpTime > rendererPeriod)
		{
			render::clearBufferXY(0.05f, 0.05f, 0.05f);
			render::clearBufferZ();

			// Update lights.
			main_ECS->updateComponent(AmbientLight_Component, (void*)this);
			main_ECS->updateComponent(DirectedLight_Component, (void*)this);
			main_ECS->updateComponent(PointLight_Component, (void*)this);
			main_ECS->updateComponent(SpotLight_Component, (void*)this);

			// Render 3D models.
			main_ECS->updateComponent(Model3D_Component, &this->activeCamera);

			// Render primitives.
			render::Camera* camera = 
				(render::Camera*)main_ECS->componentManagers[CAMERA_COMPONENT].getComponentLocation(this->activeCamera);
			mathem::Transform* cameraTransform =
				(mathem::Transform*)main_ECS->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(this->activeCamera);

			render::renderPoints(camera, cameraTransform);
			render::renderLines(camera, cameraTransform);
			render::renderTriangles(camera, cameraTransform);

			render::display();
			rendererTime = std::chrono::high_resolution_clock::now();
		}

		//onSceneUpdate(this);
	}
};


void processInput_Script(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data)
{
	float cameraSensitivity = 0.1f;
	float speed = 450;

	MiteVox_Scene* scene = (MiteVox_Scene*)data;

	InputHandler* inputHandler = scene->inputHandler;
	
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(entityID);

	render::Camera* camera = (render::Camera*)_ecs->componentManagers[CAMERA_COMPONENT].getComponentLocation(scene->activeCamera);
	camera->FOV += inputHandler->mouseDeltaScroll;

	
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		if (inputHandler->isAttached)
		{
			inputHandler->detach(render::RendererSettings.window);
		}
		else
		{
			inputHandler->attach(render::RendererSettings.window);
		}
		inputHandler->afterPush();
	}*/

	float timeForMoves = (float)inputHandler->dt;

	if (inputHandler->keyW)
	{
		transform->x += cos((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->y -= cos((transform->angleX + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->z += sin((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
	}
	else if (inputHandler->keyS)
	{
		transform->x -= cos((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->y += cos((transform->angleX + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->z -= sin((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
	}
	if (inputHandler->keyD)
	{
		transform->x += cos(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
		transform->z += sin(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
	}
	else if (inputHandler->keyA)
	{
		transform->x -= cos(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
		transform->z -= sin(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
	}

	if (inputHandler->keySpace)
	{
		transform->y += speed * timeForMoves;
	}
	else if (inputHandler->keyLShift)
	{
		transform->y -= speed * timeForMoves;
	}

	int windowWidth, windowHeight;
	inputHandler->getWindowSize(&windowWidth, &windowHeight);
	transform->angleY += (-(float)inputHandler->mouseDeltaX + windowWidth / 2) * cameraSensitivity;
	transform->angleX += (-(float)inputHandler->mouseDeltaY + windowHeight / 2) * cameraSensitivity;

	/*mathem::Vector3D direction = transform->getDirectionVector();

	render::SpotLight* spotLight =
		(render::SpotLight*)_ecs->componentManagers[SPOTLIGHT_COMPONENT].getComponentLocation(entityID);
	spotLight->angle += inputHandler->mouseDeltaScroll;
	spotLight->direction.x = direction.i;
	spotLight->direction.y = direction.j;
	spotLight->direction.z = direction.k;*/
}


void rotateCamera_Script(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data)
{
	MiteVox_Scene* scene = (MiteVox_Scene*)data;
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(entityID);

	float angle = (float)scene->currentTime * 50 + 180;

	transform->x = 800 * sin(mathem::toRadians(angle));
	transform->z = 800 * cos(mathem::toRadians(angle));

	transform->angleY = -angle + 180;
}

#endif