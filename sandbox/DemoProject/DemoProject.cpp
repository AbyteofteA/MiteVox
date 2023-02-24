
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

	Entity* entity = activeScene->entities.getElement(0);
	entity->movementProperties.inverseMass = 1.0f;
	entity->movementProperties.inverseMomentOfInertia = 0.5f;
	entity->movementProperties.restitution = 1.0f;
	//node->movementProperties->angularVelocity = { 0.1f, 0.0f, 0.0f };
	
	Entity* whiteLightEntity = new Entity();
	whiteLightEntity->renderableNode.lightType = render::LightType::POINT;
	whiteLightEntity->renderableNode.light.pointLight.lightBase = { { 1.0, 1.0, 1.0 }, 1.0, 40.0 };
	whiteLightEntity->setTranslation({ 11.0, 11.0, 11.0 });
	whiteLightEntity->collider = entity->collider;

	activeScene->entities.appendElement(whiteLightEntity);

	size_t activeEntitiesCount = activeScene->entities.getElementsCount();
	for (size_t i = 0; i < activeEntitiesCount; ++i)
	{
		activeScene->foundation->emplace(activeScene->entities.getElement(i));
	}
}

void mitevox::Engine::onUpdate() {}

void mitevox::Engine::onDestroy() {}


#include "engine/MiteVox/src/MiteVox_main.h"