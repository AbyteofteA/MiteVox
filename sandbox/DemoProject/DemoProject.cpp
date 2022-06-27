
//----------------------------------------------------------------------------------------
#define SCREEN_WIDTH		1280
#define SCREEN_HEIGHT		720
//----------------------------------------------------------------------------------------

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

#include "engine/FileIO/src/FileLoaderAsync.h"
#include "engine/FileIO/src/Formats/WavefrontOBJ/WavefrontOBJRegexPack.h"
#include "engine/FileIO/src/Formats/CodecGLTF/CodecGLTF.h"

#include "scripts.h"

#include <filesystem>
namespace fs = std::filesystem;


int basicShader = -1;
int primitiveShader = -1;
int skyboxShader = -1;

void mitevox::Engine::onCreate() 
{
	this->playground->name = "Demo";
	mitevox::Scene* myScene = this->playground->getActiveScene();
	ecs::EntityComponentSystem<entityID>* myECS = myScene->ECS;

	// Create 3D-model object.

	render::Mesh3D* mCube = nullptr;
	std::string cubeDir = settings->getResourceDir() + "/assets/cube.obj";
	fileio::fileLoader.loadAndParseAsync(cubeDir, (void**)&mCube, render::parseModel_OBJ);

	// Load images (textures).

	mitevox::Image* white = nullptr;
	mitevox::Image* white_rough = nullptr;
	mitevox::Image* light_grey = nullptr;
	mitevox::Image* error = nullptr;
	mitevox::Image* UVchecker = nullptr;
	mitevox::Image* UVchecker0 = nullptr;
	mitevox::Image* UVchecker1 = nullptr;
	mitevox::Image* SPECchecker0 = nullptr;

	std::string texturesDir = settings->getResourceDir() + "/assets/textures";
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/UVchecker.png", (void**)&UVchecker, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/UVchecker0.png", (void**)&UVchecker0, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/UVchecker1.png", (void**)&UVchecker1, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/SPECchecker0.png", (void**)&SPECchecker0, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/white.png", (void**)&white, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/white_rough.png", (void**)&white_rough, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/light_grey.png", (void**)&light_grey, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/error.png", (void**)&error, mitevox::loadImage);
	fileio::fileLoader.awaitAll();

	mitevox::Material* darkSomething = new mitevox::Material();
	darkSomething->baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	darkSomething->roughness = 1.0f;
	darkSomething->metallicity = 1.0f;
	darkSomething->specularExponent = 5;
	darkSomething->albedoMap = new mitevox::Texture(error);
	darkSomething->metallicRoughnessMap = new mitevox::Texture(SPECchecker0);
	darkSomething->illuminationModel = 2;

	NativeScript_ECS waveScript = { nullptr, waveModel_Script, nullptr };
	NativeScript_ECS rotateScript = { nullptr, rotateModel_Script, nullptr };
	NativeScript_ECS rotateLightScript = { nullptr, rotateLight_Script, nullptr };

	//----------------------------------------------------------------------------------------

	// Create subject entity.

	entityID subject0 = myECS->createEntity();
	render::Camera tmpCamera = { 50, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 100000 };
	myECS->attachComponent(subject0, myScene->Camera_Component, &tmpCamera);
	myScene->activeCamera = subject0;
	mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, -30 };
	myECS->attachComponent(subject0, myScene->Transform_Component, &tmpTransform);
	NativeScript_ECS tmpNativeScript = { nullptr, processInput_Script, nullptr };
	myECS->attachComponent(subject0, myScene->NativeScript_Component, &tmpNativeScript);
	/*render::SpotLight tmpSpotLight = { { 0, 0, 0 }, { 0, 1, 0 }, 10,
									{ 1, 1, 1, 1 }, 1, 0.0007, 0.00002 };
	ECS->attachComponent<render::SpotLight>(subject0, SpotLight_Component, &tmpSpotLight);*/

	// Create Light prefab.

	entityID Light = myECS->createPrefab("Light");
	myECS->getPrefab(Light)->attachComponent(
		myECS->componentManagers[myScene->Transform_Component]);
	mitevox::Material* light = new mitevox::Material();
	light->baseColor = { 1.0, 1.0, 1.0, 1.0 };
	light->roughness = 1.0;
	light->metallicity = 1.0;
	light->specularExponent = 8.0;
	//light->albedoMap = new fileio::Texture(white);
	//light->metallicRoughnessMap = new fileio::Texture(white);
	render::Model3D* cubeLight = new render::Model3D(mCube, light, { 3, 3, 3, 0, 0, 0, 0, 0, 0 });
	cubeLight->shaderID = basicShader;
	render::PointLight tmpPointLight = { { 0 }, { 1, 1, 1 },  1, 0.0014f, 0.000007f, 1.0, 200.0 };
	myECS->getPrefab(Light)->attachComponent(
		myECS->componentManagers[myScene->PointLight_Component], &tmpPointLight);
	myECS->getPrefab(Light)->attachComponent(
		myECS->componentManagers[myScene->NativeScript_Component]);

	// Spawn 9x9 grid of cubes.

	/*int maxX = 2;
	int maxY = 2;
	for (int i = -maxX; i <= maxX; i++)
	{
		for (int j = -maxY; j <= maxY; j++)
		{
			entityID tmpID = myECS->createEntity(Cube);
			mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, (float)50 * i, 0, (float)50 * j };
			*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
		}
	}*/

	// Spawn lights.

	float lightPos = 200;

	tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, 0 };
	entityID tmpID = myECS->createEntity(Light);
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

	/*tmpID = myECS->createEntity(Light);
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
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;*/

	tmpID = myECS->createEntity(Light);
	tmpTransform.x = 0; tmpTransform.y = 150; tmpTransform.z = 0;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	*(NativeScript_ECS*)myECS->getComponent(tmpID, myScene->NativeScript_Component) = rotateScript;
}


void mitevox::Engine::onUpdate() 
{
	auto rendererSettings = this->settings->getRendererSettings();
	render::clearBufferXY(rendererSettings->clearColor);
	render::clearBufferZ();
}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"