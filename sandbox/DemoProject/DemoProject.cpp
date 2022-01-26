
//----------------------------------------------------------------------------------------
#define SCREEN_WIDTH		1280
#define SCREEN_HEIGHT		720
//----------------------------------------------------------------------------------------

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/FileIO/src/FileLoaderAsync.h"

#include "engine/FileIO/src/Formats/WavefrontOBJ/WavefrontOBJRegexPack.h"

#include "scripts.h"

#include <filesystem>
namespace fs = std::filesystem;


int basicShader = -1;
int primitiveShader = -1;
int skyboxShader = -1;

void mitevox::Engine::onCreate() 
{
	this->playground->name = "Demo";
	this->playground->createMainScene("Demo Scene", settings);
	mitevox::Scene* myScene = this->playground->getActiveScene();
	ecs::EntityComponentSystem<entityID>* myECS = myScene->ECS;

	// Compile shaders.

	std::string shadersDir = settings->getResourceDir() + "/shaders";
	basicShader = render::createShader("Basic Shader", shadersDir + "/basic/basic");
	skyboxShader = render::createShader("Skybox Shader", shadersDir + "/skybox/skybox");
	primitiveShader = render::createShader("Primitive Shader", shadersDir + "/primitive/primitive");

	this->settings->getRendererSettings()->primitiveShaderID = primitiveShader;

	// Create 3D-model object.

	render::Mesh3D* mCube = nullptr;
	std::string cubeDir = settings->getResourceDir() + "/assets/cube.obj";
	fileio::fileLoader.loadAndParseAsync(cubeDir, (void**)&mCube, render::parseModel_OBJ);

	// Load images (textures).

	fileio::Image* white = nullptr;
	fileio::Image* white_rough = nullptr;
	fileio::Image* light_grey = nullptr;
	fileio::Image* error = nullptr;
	fileio::Image* UVchecker = nullptr;
	fileio::Image* UVchecker0 = nullptr;
	fileio::Image* UVchecker1 = nullptr;
	fileio::Image* SPECchecker0 = nullptr;

	std::string texturesDir = settings->getResourceDir() + "/assets/textures";
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/UVchecker.png", (void**)&UVchecker, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/UVchecker0.png", (void**)&UVchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/UVchecker1.png", (void**)&UVchecker1, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/SPECchecker0.png", (void**)&SPECchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/white.png", (void**)&white, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/white_rough.png", (void**)&white_rough, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/light_grey.png", (void**)&light_grey, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/error.png", (void**)&error, fileio::loadImage);
	fileio::fileLoader.awaitAll();

	fileio::Material* darkSomething = new fileio::Material();
	darkSomething->baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	darkSomething->roughness = 1.0f;
	darkSomething->metallicity = 1.0f;
	darkSomething->specularExponent = 5;
	darkSomething->albedoMap = new fileio::Texture(error);
	darkSomething->metallicRoughnessMap = new fileio::Texture(SPECchecker0);
	darkSomething->illuminationModel = 2;

	NativeScript_ECS waveScript = { nullptr, waveModel_Script, nullptr };
	NativeScript_ECS rotateScript = { nullptr, rotateModel_Script, nullptr };
	NativeScript_ECS rotateLightScript = { nullptr, rotateLight_Script, nullptr };

	//----------------------------------------------------------------------------------------

	// Create subject entity.

	entityID subject0 = myECS->createEntity();
	render::Camera tmpCamera = { 50, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 100000 };
	myECS->attachComponent(subject0, myScene->Camera_Component, &tmpCamera);
	myScene->activeCamera = subject0;
	mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, -700 };
	myECS->attachComponent(subject0, myScene->Transform_Component, &tmpTransform);
	NativeScript_ECS tmpNativeScript = { nullptr, processInput_Script, nullptr };
	myECS->attachComponent(subject0, myScene->NativeScript_Component, &tmpNativeScript);
	/*render::SpotLight tmpSpotLight = { { 0, 0, 0 }, { 0, 1, 0 }, 10,
									{ 1, 1, 1, 1 }, 1, 0.0007, 0.00002 };
	ECS->attachComponent<render::SpotLight>(subject0, SpotLight_Component, &tmpSpotLight);*/

	// Create Cube prefab.

	entityID Cube = myECS->createPrefab("Cube");
	myECS->getPrefab(Cube)->attachComponent(
		myECS->componentManagers[myScene->Transform_Component]);
	myECS->getPrefab(Cube)->attachComponent(
		myECS->componentManagers[myScene->NativeScript_Component], &waveScript);

	render::Model3D* cube = new render::Model3D(
		mCube, darkSomething, mathem::Transform(25, 25, 25, 0, 90, 0, 0, 0, 0));
	cube->shaderID = basicShader;
	myECS->getPrefab(Cube)->attachComponent(
		myECS->componentManagers[myScene->Model3D_Component], cube);

	render::Model3D* md = (render::Model3D*)myECS->getPrefab(Cube)->components[MODEL3D_COMPONENT];
	md->shaderID = md->shaderID;

	// Create Light prefab.

	entityID Light = myECS->createPrefab("Light");
	myECS->getPrefab(Light)->attachComponent(
		myECS->componentManagers[myScene->Transform_Component]);
	fileio::Material* light = new fileio::Material();
	light->baseColor = { 1.0, 1.0, 1.0, 1.0 };
	light->roughness = 1.0;
	light->metallicity = 1.0;
	light->specularExponent = 8.0;
	//light->albedoMap = new fileio::Texture(white);
	//light->metallicRoughnessMap = new fileio::Texture(white);
	render::Model3D* cubeLight = new render::Model3D(mCube, light, { 3, 3, 3, 0, 0, 0, 0, 0, 0 });
	cubeLight->shaderID = basicShader;
	myECS->getPrefab(Light)->attachComponent(
		myECS->componentManagers[myScene->Model3D_Component], cubeLight);
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

	float lightPos = 450;

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
	tmpTransform.x = 0; tmpTransform.y = 300; tmpTransform.z = 0;
	*(mathem::Transform*)myECS->getComponent(tmpID, myScene->Transform_Component) = tmpTransform;
	*(NativeScript_ECS*)myECS->getComponent(tmpID, myScene->NativeScript_Component) = rotateScript;
}


void mitevox::Engine::onUpdate() 
{
	auto rendererSettings = this->settings->getRendererSettings();
	render::clearBufferXY(rendererSettings->clearColor);
	render::clearBufferZ();

	float cubeSize = 100;
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

	render::drawLine(rendererSettings, point1, point2);
	render::drawLine(rendererSettings, point2, point3);
	render::drawLine(rendererSettings, point3, point4);
	render::drawLine(rendererSettings, point4, point5);
	render::drawLine(rendererSettings, point5, point6);
	render::drawLine(rendererSettings, point6, point7);
	render::drawLine(rendererSettings, point7, point8);
	render::drawLine(rendererSettings, point8, point1);

	render::drawLine(rendererSettings, point1, point4);
	render::drawLine(rendererSettings, point3, point6);
	render::drawLine(rendererSettings, point5, point8);
	render::drawLine(rendererSettings, point2, point7);
}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"