
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
	mitevox::Image* error = nullptr;

	std::string texturesDir = settings->getResourceDir() + "/assets/textures";
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/white.png", (void**)&white, mitevox::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		texturesDir + "/error.png", (void**)&error, mitevox::loadImage);
	fileio::fileLoader.awaitAll();

	mitevox::Material* darkSomething = new mitevox::Material();
	darkSomething->baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	darkSomething->roughness = 1.0f;
	darkSomething->metallicity = 1.0f;
	darkSomething->specularExponent = 5;
	darkSomething->albedoMap = new mitevox::Texture(error);
	darkSomething->metallicRoughnessMap = new mitevox::Texture(white);
	darkSomething->illuminationModel = 2;

	NativeScript_ECS waveScript = { nullptr, waveModel_Script, nullptr };
	NativeScript_ECS rotateScript = { nullptr, rotateModel_Script, nullptr };

	//----------------------------------------------------------------------------------------

	// Create subject entity.

	entityID subject0 = myECS->createEntity();
	myScene->activeCamera = subject0;
	mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, 0, 0, -10 };
	myECS->attachComponent(subject0, myScene->Transform_Component, &tmpTransform);
	NativeScript_ECS tmpNativeScript = { nullptr, processInput_Script, nullptr };
	myECS->attachComponent(subject0, myScene->NativeScript_Component, &tmpNativeScript);

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

	Node* redLightNode = new Node();
	redLightNode->lightType = render::LightType::POINT;
	redLightNode->light.pointLight.lightBase = { { 1.0, 0.0, 0.0 }, 1.0, 15.0 };
	redLightNode->transform.translation = { 5.0, 0.0, 0.0 };

	Node* greenLightNode = new Node();
	greenLightNode->lightType = render::LightType::POINT;
	greenLightNode->light.pointLight.lightBase = { { 0.0, 1.0, 0.0 }, 1.0, 15.0 };
	greenLightNode->transform.translation = { 0.0, 5.0, 0.0 };

	Node* blueLightNode = new Node();
	blueLightNode->lightType = render::LightType::POINT;
	blueLightNode->light.pointLight.lightBase = { { 0.0, 0.0, 1.0 }, 1.0, 15.0 };
	blueLightNode->transform.translation = { 0.0, 0.0, 5.0 };

	Node* rootNode = playground->nodes.getElement(0);
	rootNode->children.appendElement(redLightNode);
	rootNode->children.appendElement(greenLightNode);
	rootNode->children.appendElement(blueLightNode);
}

void mitevox::Engine::onUpdate() {}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"