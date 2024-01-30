
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
	
	auto entity = MiteVoxAPI::getActiveCameraEntity();
}

void mitevox::Engine::onUpdate() {}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"