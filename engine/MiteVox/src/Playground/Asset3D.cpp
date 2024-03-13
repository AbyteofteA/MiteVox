#include "Asset3D.h"

#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace mitevox
{
	Asset3D::Asset3D(std::string _name)
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

	Asset3D::~Asset3D()
	{
		size_t scenesCount = scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			delete scenes.getElement(i);
		}
	}

	void Asset3D::updateAssets()
	{
		size_t scenesCount = scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			Scene* scene = scenes.getElement(i);
			// TODO: 
		}
	}

	// TODO: How to add assets?

	size_t Asset3D::createDefaultScene()
	{
		size_t index = createActiveScene("Default Scene");
		Scene* defaultScene = scenes.getElement(index);


		return index;
	}

	size_t Asset3D::createScene(std::string name)
	{
		Scene* newScene = new Scene();
		newScene->name = name;
		scenes.appendElement(newScene);
		return scenes.getElementsCount() - 1;
	}

	size_t Asset3D::createActiveScene(std::string name)
	{
		size_t index = createScene(name);
		return activeScene = index;
	}

	void Asset3D::deleteScene(size_t index)
	{
		delete scenes.getElement(index);
		scenes.removeElement(index);
	}

	Scene* Asset3D::getActiveScene()
	{
		return scenes.getElement(activeScene);
	}

	size_t Asset3D::addEntity(Entity* entity)
	{
		Scene* activeScene = getActiveScene();
		size_t entitiesCount = activeScene->entities.getElementsCount();
		activeScene->entities.appendElement(entity);
		return entitiesCount;
	}

	void Asset3D::removeEntity(Entity* entity)
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
