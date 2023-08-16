
#include "Playground.h"

#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Playground::Playground(std::string _name)
	{
		name = _name;

		buffers.reserve(128);
		bufferViews.reserve(128);
		accessors.reserve(128);
		cameras.reserve(8);
		imageSamplers.reserve(128);
		images.reserve(128);
		textures.reserve(128);
		materials.reserve(128);
		physicalMaterials.reserve(128);
		meshes.reserve(128);
		nodes.reserve(128);
		scenes.reserve(128);

		// Create default assets
		//safety::SafeArray<safety::SafeByteArray*> buffers;
		//safety::SafeArray<BufferView*> bufferViews;
		//safety::SafeArray<BufferViewAccessor*> accessors;

		//render::Camera* camera = new render::Camera();
		//// TODO: 
		//cameras.appendElement(camera);
		//imageSamplers.appendElement(ImageSampler()); // TODO: 
		//images.appendElement(Image()); // TODO: 
		//textures.appendElement(Texture()); // TODO: 
		//materials.appendElement(Material()); // TODO: 
		//physicalMaterial.appendElement(PhysicalMaterial("Default PhysicalMaterial", 1.0f, 0.0f, 1.0f, 1.0f)); // TODO: 
		////safety::SafeArray<Mesh*> meshes;
		////safety::SafeArray<Node*> nodes;
		//scenes.appendElement(new Scene()); // TODO: 

		//safety::SafeArray<Animation*> animations;
		//safety::SafeArray<SkeletonBase*> skeletons;
	}

	Playground::~Playground()
	{
		size_t scenesCount = scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			delete scenes.getElement(i);
		}
	}

	void Playground::updateAssets()
	{
		size_t scenesCount = scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			Scene* scene = scenes.getElement(i);
			// TODO: 
		}
	}

	// TODO: How to add assets?

	size_t Playground::createDefaultScene(EngineSettings* settings)
	{
		size_t index = createActiveScene("Default Scene", settings);
		Scene* defaultScene = scenes.getElement(index);


		return index;
	}

	size_t Playground::createScene(std::string name, EngineSettings* settings)
	{
		Scene* newScene = new Scene(settings);
		newScene->name = name;
		scenes.appendElement(newScene);
		return scenes.getElementsCount() - 1;
	}

	size_t Playground::createActiveScene(std::string name, EngineSettings* settings)
	{
		size_t index = createScene(name, settings);
		return activeScene = index;
	}

	void Playground::deleteScene(size_t index)
	{
		delete scenes.getElement(index);
		scenes.removeElement(index);
	}

	Scene* Playground::getActiveScene()
	{
		return scenes.getElement(activeScene);
	}

	size_t Playground::addEntity(Entity* entity)
	{
		Scene* activeScene = getActiveScene();
		size_t entitiesCount = activeScene->entities.getElementsCount();
		activeScene->entities.appendElement(entity);
		activeScene->foundation->emplace(entity); // TODO: delete
		return entitiesCount;
	}

	void Playground::removeEntity(Entity* entity)
	{
		Scene* activeScene = getActiveScene();
		size_t entitiesCount = activeScene->entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			if (entity == activeScene->entities.getElement(i))
			{
				activeScene->entities.removeElement(i);
				return;
			}
		}
	}
}
