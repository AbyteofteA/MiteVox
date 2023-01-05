
//----------------------------------------------------------------------------------------
#define SCREEN_WIDTH		1280
#define SCREEN_HEIGHT		720
//----------------------------------------------------------------------------------------

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

#include "engine/FileIO/src/FileLoaderAsync.h"
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

	Node* whiteLightNode = new Node();
	whiteLightNode->lightType = render::LightType::POINT;
	whiteLightNode->light.pointLight.lightBase = { { 1.0, 1.0, 1.0 }, 1.0, 20.0 };
	whiteLightNode->transform.translation = { 5.0, 5.0, 5.0 };

	Node* redLightNode = new Node();
	redLightNode->lightType = render::LightType::POINT;
	redLightNode->light.pointLight.lightBase = { { 1.0, 1.0, 1.0 }, 1.0, 10.0 };
	redLightNode->transform.translation = { 0.0, 0.0, -5.0 };

	Node* greenLightNode = new Node();
	greenLightNode->lightType = render::LightType::POINT;
	greenLightNode->light.pointLight.lightBase = { { 1.0, 1.0, 1.0 }, 1.0, 10.0 };
	greenLightNode->transform.translation = { 0.0, 5.0, 0.0 };

	Node* blueLightNode = new Node();
	blueLightNode->lightType = render::LightType::POINT;
	blueLightNode->light.pointLight.lightBase = { { 1.0, 1.0, 1.0 }, 1.0, 10.0 };
	blueLightNode->transform.translation = { 0.0, 0.0, 5.0 };

	playground->nodes.appendElement(redLightNode);
	playground->nodes.appendElement(greenLightNode);
	playground->nodes.appendElement(blueLightNode);

	mitevox::Scene* activeScene = this->playground->getActiveScene();
	activeScene->nodes.appendElement(whiteLightNode);
	/*activeScene->nodes.appendElement(redLightNode);
	activeScene->nodes.appendElement(greenLightNode);
	activeScene->nodes.appendElement(blueLightNode);*/

	activeScene->scripts.appendElement(processInput_Script);
}

void mitevox::Engine::onUpdate() {}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"