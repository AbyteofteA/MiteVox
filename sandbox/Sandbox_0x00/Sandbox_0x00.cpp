﻿
//----------------------------------------------------------------------------------------
#define SCREEN_WIDTH		1280
#define SCREEN_HEIGHT		720

//#define DEBUG_ON
//----------------------------------------------------------------------------------------

#include "MiteVox/src/MiteVox.h"

#include "scripts.h"


void mitevox::MiteVox_Engine::onCreate() 
{
	this->activeScene = this->createScene();
	mitevox::MiteVox_Scene* myScene = this->getActiveScene();
	ecs::ECS* myECS = this->getActiveScene()->ECS;

	// Create 3D-model objects.
	render::Mesh3D* mCube = nullptr;
#ifdef DEBUG_ON
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\cube.obj", (void**)&mCube, render::parseModel_OBJ);
#else
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\cube.obj", (void**)&mCube, render::parseModel_OBJ);
#endif

	// Load images (textures).
	fileio::Image* white = nullptr;
	fileio::Image* white_rough = nullptr;
	fileio::Image* light_grey = nullptr;
	fileio::Image* black = nullptr;
	fileio::Image* UVchecker0 = nullptr;
	fileio::Image* UVchecker1 = nullptr;
	fileio::Image* SPECchecker0 = nullptr;

#ifdef DEBUG_ON
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\UVchecker0.png", (void**)&UVchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\UVchecker1.png", (void**)&UVchecker1, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\SPECchecker0.png", (void**)&SPECchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\white.png", (void**)&white, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\white_rough.png", (void**)&white_rough, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\light_grey.png", (void**)&light_grey, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\engine\\Renderer\\assets\\textures\\black.png", (void**)&black, fileio::loadImage);
#else
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\UVchecker0.png", (void**)&UVchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\UVchecker1.png", (void**)&UVchecker1, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\SPECchecker0.png", (void**)&SPECchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\white.png", (void**)&white, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\white_rough.png", (void**)&white_rough, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\light_grey.png", (void**)&light_grey, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\..\\..\\engine\\Renderer\\assets\\textures\\black.png", (void**)&black, fileio::loadImage);
#endif
	fileio::fileLoader.awaitAll();

	render::Material* darkSomething = new render::Material();
	darkSomething->albedo = { 0.0f, 0.0f, 0.0f };
	darkSomething->roughness = { 0.7f, 0.7f, 0.7f };
	darkSomething->metallicity = { 0.5f, 0.5f, 0.5f };
	darkSomething->specularExponent = 5;
	darkSomething->albedoMap = *SPECchecker0;
	darkSomething->metallicMap = *white;

	NativeScript_ECS waveScript = { doNothing, waveModel_Script, doNothing };
	NativeScript_ECS rotateScript = { doNothing, rotateModel_Script, doNothing };
	NativeScript_ECS rotateLightScript = { doNothing, rotateLight_Script, doNothing };

	//----------------------------------------------------------------------------------------

	// Create subject entity.

	unsigned long subject0 = myECS->createEntity();
	render::Camera tmpCamera = { 50, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 100000 };
	myECS->attachComponent(subject0, myScene->Camera_Component, &tmpCamera);
	myScene->activeCamera = subject0;
	mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, -700 };
	myECS->attachComponent(subject0, myScene->Transform_Component, &tmpTransform);
	NativeScript_ECS tmpNativeScript = { doNothing, processInput_Script, doNothing };
	myECS->attachComponent(subject0, myScene->NativeScript_Component, &tmpNativeScript);
	/*render::SpotLight tmpSpotLight = { { 0, 0, 0 }, { 0, 1, 0 }, 10,
									{ 1, 1, 1, 1 }, 1, 0.0007, 0.00002 };
	ECS->attachComponent<render::SpotLight>(subject0, SpotLight_Component, &tmpSpotLight);*/

	// Create Cube prefab.

	COMPONENT_TYPE Cube = myECS->createPrefab("Cube");
	myECS->getPrefab(Cube)->attachComponent(
		&myECS->componentManagers[myScene->Transform_Component]);
	myECS->getPrefab(Cube)->attachComponent(
		&myECS->componentManagers[myScene->NativeScript_Component], &waveScript);

	render::Model3D* cube = new render::Model3D(
		mCube, darkSomething, mathem::Transform(25, 25, 25, 0, 90, 0, 0, 0, 0));
	cube->shaderID = 0;
	myECS->getPrefab(Cube)->attachComponent(
		&myECS->componentManagers[myScene->Model3D_Component], cube);

	render::Model3D* md = (render::Model3D*)myECS->getPrefab(Cube)->components[MODEL3D_COMPONENT];
	md->shaderID = md->shaderID;

	// Create Light prefab.

	COMPONENT_TYPE Light = myECS->createPrefab("Light");
	myECS->getPrefab(Light)->attachComponent(
		&myECS->componentManagers[myScene->Transform_Component]);
	render::Material* light = new render::Material();
	light->albedo = { 1, 1, 1 };
	light->roughness = { 0, 0, 0 };
	light->metallicity = { 0, 0, 0 };
	light->specularExponent = 1;
	light->albedoMap = *white;
	light->metallicMap = *white;
	render::Model3D* cubeLight = new render::Model3D(mCube, light, { 3, 3, 3, 0, 0, 0, 0, 0, 0 });
	cubeLight->shaderID = 0;
	myECS->getPrefab(Light)->attachComponent(
		&myECS->componentManagers[myScene->Model3D_Component], cubeLight);
	render::PointLight tmpPointLight = { { 0 }, { 1, 1, 1, 1 },  1, 0.0014f, 0.000007f };
	myECS->getPrefab(Light)->attachComponent(
		&myECS->componentManagers[myScene->PointLight_Component], &tmpPointLight);
	myECS->getPrefab(Light)->attachComponent(
		&myECS->componentManagers[myScene->NativeScript_Component]);

	md = (render::Model3D*)myECS->getPrefab(Cube)->components[MODEL3D_COMPONENT];
	md->shaderID = md->shaderID;

	int maxX = 3;
	int maxY = 3;
	for (int i = -maxX; i <= maxX; i++)
	{
		for (int j = -maxY; j <= maxY; j++)
		{
			COMPONENT_TYPE tmpID = myECS->createEntity(Cube);
			mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, (float)50 * i, 0, (float)50 * j };
			*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
		}
	}
	float lightPos = 450;

	tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, 0 };
	COMPONENT_TYPE tmpID = myECS->createEntity(Light);
	tmpTransform.x = lightPos; tmpTransform.y = lightPos; tmpTransform.z = lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	tmpID = myECS->createEntity(Light);
	tmpTransform.x = -lightPos; tmpTransform.y = lightPos; tmpTransform.z = lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	tmpID = myECS->createEntity(Light);
	tmpTransform.x = -lightPos; tmpTransform.y = lightPos; tmpTransform.z = -lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	tmpID = myECS->createEntity(Light);
	tmpTransform.x = lightPos; tmpTransform.y = lightPos; tmpTransform.z = -lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;

	tmpID = myECS->createEntity(Light);
	tmpTransform.x = lightPos; tmpTransform.y = -lightPos; tmpTransform.z = lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	tmpID = myECS->createEntity(Light);
	tmpTransform.x = -lightPos; tmpTransform.y = -lightPos; tmpTransform.z = lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	tmpID = myECS->createEntity(Light);
	tmpTransform.x = -lightPos; tmpTransform.y = -lightPos; tmpTransform.z = -lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	tmpID = myECS->createEntity(Light);
	tmpTransform.x = lightPos; tmpTransform.y = -lightPos; tmpTransform.z = -lightPos;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;

	tmpID = myECS->createEntity(Light);
	tmpTransform.x = 0; tmpTransform.y = 300; tmpTransform.z = 0;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	*(NativeScript_ECS*)myECS->getComponent(tmpID, myScene->NativeScript_Component) = rotateScript;
}


void mitevox::MiteVox_Engine::onUpdate() 
{
	float cubeSize = 1000;
	render::ColorRGBAf grey = { 0.3f, 0.3f, 0.3f, 1 };
	render::ColorRGBAf red = { 1, 0, 0, 1 };
	render::ColorRGBAf green = { 0, 1, 0, 1 };
	render::ColorRGBAf blue = { 0, 0, 1, 1 };
	render::ColorRGBAf purple = { 0.6f, 0.2f, 0.7f, 1 };
	render::Point point1 = { {cubeSize, cubeSize, cubeSize}, grey };
	render::Point point2 = { {cubeSize, -cubeSize, cubeSize}, grey };
	render::Point point3 = { {cubeSize, -cubeSize, -cubeSize}, grey };
	render::Point point4 = { {cubeSize, cubeSize, -cubeSize}, grey };
	render::Point point5 = { {-cubeSize, cubeSize, -cubeSize}, grey };
	render::Point point6 = { {-cubeSize, -cubeSize, -cubeSize}, grey };
	render::Point point7 = { {-cubeSize, -cubeSize, cubeSize}, grey };
	render::Point point8 = { {-cubeSize, cubeSize, cubeSize}, grey };

	render::drawLine(this->renderer, point1, point2);
	render::drawLine(this->renderer, point2, point3);
	render::drawLine(this->renderer, point3, point4);
	render::drawLine(this->renderer, point4, point5);
	render::drawLine(this->renderer, point5, point6);
	render::drawLine(this->renderer, point6, point7);
	render::drawLine(this->renderer, point7, point8);
	render::drawLine(this->renderer, point8, point1);

	render::drawLine(this->renderer, point1, point4);
	render::drawLine(this->renderer, point3, point6);
	render::drawLine(this->renderer, point5, point8);
	render::drawLine(this->renderer, point2, point7);
}

void mitevox::MiteVox_Engine::onDestroy() {}


#include "MiteVox/src/MiteVox_entry.h"