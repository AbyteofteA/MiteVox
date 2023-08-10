
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

	Scene* activeScene = this->playground->getActiveScene();
	if (activeScene == nullptr)
	{
		return;
	}
	
	activeScene->scripts.appendElement(processInput_Script);

	MiteVoxAPI::createPointLightEntity({ 11.0f, 11.0f, 11.0f }, { 1.0f, 1.0f, 1.0f });
	MiteVoxAPI::createCube("Cube", { 2.0f, 0.0f, 7.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
	MiteVoxAPI::createCube("Cube", { 2.0f, 1.0f, 7.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	MiteVoxAPI::createCube("Cube", { 2.0f, 2.0f, 7.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
	MiteVoxAPI::createCube("Cube", { 2.0f, 3.0f, 7.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	MiteVoxAPI::createPlane("Plane", { 1.0f, 1.0f, 1.0f, 1.0f });
}

void mitevox::Engine::onUpdate() {}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"