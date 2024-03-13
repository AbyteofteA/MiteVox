#include "Asset3DCodecGLTF.h"

#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/BufferLayoutCodec/BufferViewCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/BufferLayoutCodec/BufferViewAccessorCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/MaterialCodec/ImageCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/MaterialCodec/ImageSamplerCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/MaterialCodec/MaterialCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/MeshCodec/MeshCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/SceneCodec/NodeCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/SceneCodec/SceneCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/AnimationCodec/AnimationCodec.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/SkeletonCodec/NodeBasedSkeletonCodec.h"
#include "engine/Math/src/Utilities/Convertations.h"

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	void collectExtensions(mitevox::Asset3D* asset3D, JSON* glTFJSON);
	bool extensionsAreSupported(mitevox::Asset3D* asset3D);
	void collectCameras(mitevox::Asset3D* asset3DResult, JSON* camerasArrayJSON);
	void collectBuffers(mitevox::Asset3D* asset3DResult, JSON* buffersArrayJSON, std::string filePath);
	void collectBufferViews(mitevox::Asset3D* asset3DResult, JSON* bufferViewsArrayJSON);
	void collectAccessors(mitevox::Asset3D* asset3DResult, JSON* accessorsArrayJSON);
	void collectImageSamplers(mitevox::Asset3D* asset3DResult, JSON* imageSamplersArrayJSON);
	void collectImages(mitevox::Asset3D* asset3DResult, JSON* imagesArrayJSON, std::string filePath);
	void collectTextures(mitevox::Asset3D* asset3DResult, JSON* texturesArrayJSON);
	void collectMaterials(mitevox::Asset3D* asset3DResult, JSON* materialsArrayJSON);
	void collectMeshes(mitevox::Asset3D* asset3DResult, JSON* meshesArrayJSON);
	void collectNodes(mitevox::Asset3D* asset3DResult, JSON* nodesArrayJSON);
	void collectScenes(mitevox::Asset3D* asset3DResult, JSON* scenesArrayJSON);
	void collectAnimations(mitevox::Asset3D* asset3DResult, JSON* animationsArrayJSON);
	void collectSkins(mitevox::Asset3D* asset3DResult, JSON* skinsArrayJSON);
	void resolveSkeletonsPointers(mitevox::Asset3D* asset3DResult);
	void preparePlayground(mitevox::Asset3D* asset3DResult);

	void saveExtensions(JSON* glTFJSON, mitevox::Asset3D* asset3D);
	void saveCameras(mitevox::Asset3D* asset3D, JSON* camerasArrayJSONResult);
	void saveBuffers(mitevox::Asset3D* asset3D, JSON* buffersArrayJSONResult, std::string filePath);
	void saveBufferViews(mitevox::Asset3D* asset3D, JSON* bufferViewsArrayJSONResult);
	void saveAccessors(mitevox::Asset3D* asset3D, JSON* accessorsArrayJSONResult);
	void saveImageSamplers(mitevox::Asset3D* asset3D, JSON* imageSamplersArrayJSONResult);
	void saveImages(mitevox::Asset3D* asset3D, JSON* imagesArrayJSONResult, std::string filePath);
	void saveTextures(mitevox::Asset3D* asset3D, JSON* texturesArrayJSONResult);
	void saveMaterials(mitevox::Asset3D* asset3D, JSON* materialsArrayJSONResult);
	void saveMeshes(mitevox::Asset3D* asset3D, JSON* meshesArrayJSONResult);
	void saveNodes(mitevox::Asset3D* asset3D, JSON* nodesArrayJSONResult);
	void saveScenes(mitevox::Asset3D* asset3D, JSON* scenesArrayJSONResult);
	void saveAnimations(mitevox::Asset3D* asset3D, JSON* animationsArrayJSONResult);
	void saveSkins(mitevox::Asset3D* asset3D, JSON* skinsArrayJSONResult);

	DeserializationStatus fromGLTF(std::string filePath, mitevox::Asset3D* asset3DResult)
	{
		JSON* assetJSON = new JSON();
		assetJSON->readFromFile(filePath);
		DeserializationStatus status = fromGLTF(filePath, asset3DResult, assetJSON);
		delete assetJSON;
		return status;
	}

	SerializationStatus toGLTF(std::string filePath, mitevox::Asset3D* asset3D)
	{
		JSON* assetJSON = new JSON();
		SerializationStatus status = toGLTF(filePath, asset3D, assetJSON);
		assetJSON->saveToFile(filePath);
		return status;
	}

	DeserializationStatus fromGLTF(std::string filePath, mitevox::Asset3D* asset3DResult, JSON* asset3DJSON)
	{
		JSON* assetJSON = asset3DJSON->getFieldObject("asset");
		asset3DResult->assetInfo.version = assetJSON->getFieldStringOrDefault("version", "");
		asset3DResult->assetInfo.minVersion = assetJSON->getFieldStringOrDefault("minVersion", "");
		asset3DResult->assetInfo.generator = assetJSON->getFieldStringOrDefault("generator", "");
		asset3DResult->assetInfo.copyright = assetJSON->getFieldStringOrDefault("copyright", "");

		collectExtensions(asset3DResult, asset3DJSON);
		if (extensionsAreSupported(asset3DResult) == false)
		{
			return DeserializationStatus::ERROR_UNKNOWN;
		}

		if (JSON* numberJSON = asset3DJSON->getField("scene"))
		{
			asset3DResult->activeScene = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
		}

		JSON* camerasArrayJSON = asset3DJSON->getFieldArray("cameras");
		collectCameras(asset3DResult, camerasArrayJSON);
		JSON* buffersArrayJSON = asset3DJSON->getFieldArray("buffers");
		collectBuffers(asset3DResult, buffersArrayJSON, filePath);
		JSON* bufferViewsArrayJSON = asset3DJSON->getFieldArray("bufferViews");
		collectBufferViews(asset3DResult, bufferViewsArrayJSON);
		JSON* accessorsArrayJSON = asset3DJSON->getFieldArray("accessors");
		collectAccessors(asset3DResult, accessorsArrayJSON);
		JSON* imageSamplerJSON = asset3DJSON->getFieldArray("samplers");
		collectImageSamplers(asset3DResult, imageSamplerJSON);
		JSON* imagesJSON = asset3DJSON->getFieldArray("images");
		collectImages(asset3DResult, imagesJSON, filePath);
		JSON* texturesJSON = asset3DJSON->getFieldArray("textures");
		collectTextures(asset3DResult, texturesJSON);
		JSON* materialsJSON = asset3DJSON->getFieldArray("materials");
		collectMaterials(asset3DResult, materialsJSON);
		JSON* meshesArrayJSON = asset3DJSON->getFieldArray("meshes");
		collectMeshes(asset3DResult, meshesArrayJSON);
		JSON* nodesArrayJSON = asset3DJSON->getFieldArray("nodes");
		collectNodes(asset3DResult, nodesArrayJSON);
		JSON* skinsArrayJSON = asset3DJSON->getFieldArray("skins");
		collectSkins(asset3DResult, skinsArrayJSON);
		resolveSkeletonsPointers(asset3DResult);
		JSON* scenesArrayJSON = asset3DJSON->getFieldArray("scenes");
		collectScenes(asset3DResult, scenesArrayJSON);
		JSON* animationsArrayJSON = asset3DJSON->getFieldArray("animations");
		collectAnimations(asset3DResult, animationsArrayJSON);

		preparePlayground(asset3DResult);

		return DeserializationStatus::COMPLETE;
	}

	SerializationStatus toGLTF(std::string filePath, mitevox::Asset3D* asset3D, JSON* asset3DJSONResult)
	{
		JSON* assetJSON = asset3DJSONResult->setFieldType("asset", JSONtype::OBJECT);
		assetJSON->setField("version", std::string("2.0"));
		//assetJSON->setField("generator", std::string("MiteVox"));

		saveExtensions(asset3DJSONResult, asset3D);

		if (asset3D->activeScene >= 0)
		{
			asset3DJSONResult->setField("scene", (double)asset3D->activeScene);
		}

		if (asset3D->cameras.getElementsCount() > 0)
		{
			JSON* camerasArrayJSONResult = asset3DJSONResult->setFieldType("cameras", JSONtype::ARRAY);
			saveCameras(asset3D, camerasArrayJSONResult);
		}
		if (asset3D->buffers.getElementsCount() > 0)
		{
			JSON* buffersArrayJSONResult = asset3DJSONResult->setFieldType("buffers", JSONtype::ARRAY);
			saveBuffers(asset3D, buffersArrayJSONResult, filePath);
		}
		if (asset3D->bufferViews.getElementsCount() > 0)
		{
			JSON* bufferViewsArrayJSONResult = asset3DJSONResult->setFieldType("bufferViews", JSONtype::ARRAY);
			saveBufferViews(asset3D, bufferViewsArrayJSONResult);
		}
		if (asset3D->accessors.getElementsCount() > 0)
		{
			JSON* accessorsArrayJSONResult = asset3DJSONResult->setFieldType("accessors", JSONtype::ARRAY);
			saveAccessors(asset3D, accessorsArrayJSONResult);
		}
		if (asset3D->imageSamplers.getElementsCount() > 0)
		{
			JSON* imageSamplersArrayJSONResult = asset3DJSONResult->setFieldType("samplers", JSONtype::ARRAY);
			saveImageSamplers(asset3D, imageSamplersArrayJSONResult);
		}
		if (asset3D->images.getElementsCount() > 0)
		{
			JSON* imagesArrayJSONResult = asset3DJSONResult->setFieldType("images", JSONtype::ARRAY);
			saveImages(asset3D, imagesArrayJSONResult, filePath);
		}
		if (asset3D->textures.getElementsCount() > 0)
		{
			JSON* texturesArrayJSONResult = asset3DJSONResult->setFieldType("textures", JSONtype::ARRAY);
			saveTextures(asset3D, texturesArrayJSONResult);
		}
		if (asset3D->materials.getElementsCount() > 0)
		{
			JSON* materialsArrayJSONResult = asset3DJSONResult->setFieldType("materials", JSONtype::ARRAY);
			saveMaterials(asset3D, materialsArrayJSONResult);
		}
		if (asset3D->meshes.getElementsCount() > 0)
		{
			JSON* meshesArrayJSONResult = asset3DJSONResult->setFieldType("meshes", JSONtype::ARRAY);
			saveMeshes(asset3D, meshesArrayJSONResult);
		}
		if (asset3D->nodes.getElementsCount() > 0)
		{
			JSON* nodesArrayJSONResult = asset3DJSONResult->setFieldType("nodes", JSONtype::ARRAY);
			saveNodes(asset3D, nodesArrayJSONResult);
		}
		if (asset3D->skeletons.getElementsCount() > 0)
		{
			JSON* skinsArrayJSONResult = asset3DJSONResult->setFieldType("skins", JSONtype::ARRAY);
			saveSkins(asset3D, skinsArrayJSONResult);
		}
		if (asset3D->scenes.getElementsCount() > 0)
		{
			JSON* scenesArrayJSONResult = asset3DJSONResult->setFieldType("scenes", JSONtype::ARRAY);
			saveScenes(asset3D, scenesArrayJSONResult);
		}
		if (asset3D->animations.getElementsCount() > 0)
		{
			JSON* animationsArrayJSONResult = asset3DJSONResult->setFieldType("animations", JSONtype::ARRAY);
			saveAnimations(asset3D, animationsArrayJSONResult);
		}

		return SerializationStatus::COMPLETE;
	}

	void collectExtensions(mitevox::Asset3D* asset3D, fileio::JSON* glTFJSON)
	{
		if (glTFJSON == nullptr)
		{
			return;
		}

		fileio::JSON* extensionsUsedJSON = glTFJSON->getFieldArray("extensionsUsed");
		if (extensionsUsedJSON != nullptr)
		{
			extensionsUsedJSON->toStringArrayOrDefault(&asset3D->extensionsUsed, "ERROR");
		}

		fileio::JSON* extensionsRequiredJSON = glTFJSON->getFieldArray("extensionsRequired");
		if (extensionsRequiredJSON != nullptr)
		{
			extensionsRequiredJSON->toStringArrayOrDefault(&asset3D->extensionsRequired, "ERROR");
		}
	}

	bool extensionsAreSupported(mitevox::Asset3D* asset3D)
	{
		int64_t extensionsUsedCount = asset3D->extensionsUsed.getElementsCount();
		int64_t extensionsRequiredCount = asset3D->extensionsRequired.getElementsCount();
		bool extensionIsSupported = false;
		for (int64_t indexRequired = 0; indexRequired < extensionsRequiredCount; ++indexRequired)
		{
			extensionIsSupported = false;
			std::string extensionRequired = asset3D->extensionsRequired.getElement(indexRequired);
			for (int64_t indexUsed = 0; indexUsed < extensionsUsedCount; ++indexUsed)
			{
				std::string extensionUsed = asset3D->extensionsUsed.getElement(indexUsed);
				if (extensionRequired == extensionUsed)
				{
					extensionIsSupported = true;
					break;
				}
			}

			if (extensionIsSupported == false)
			{
				return false;
			}
		}
		return true;
	}

	void collectCameras(mitevox::Asset3D* asset3DResult, fileio::JSON* camerasArrayJSON)
	{
		if (camerasArrayJSON == nullptr)
		{
			return;
		}

		int64_t camerasCount = camerasArrayJSON->getArraySize();
		asset3DResult->cameras.resize(camerasCount);

		for (int64_t i = 0; i < camerasCount; ++i)
		{
			JSON* cameraJSON = camerasArrayJSON->getArrayItem(i);

			render::Camera* camera = new render::Camera();
			camera->_name = cameraJSON->getFieldStringOrDefault("name", "Untitled");
			std::string cameraType = cameraJSON->getFieldStringOrDefault("type", "ERROR");
			if (cameraType == "perspective")
			{
				camera->_type = render::CameraType::PERPECTIVE;
				JSON* perspectiveCameraJSON = cameraJSON->getFieldObject("perspective");

				double aspectRatio = perspectiveCameraJSON->getFieldNumberOrDefault("aspectRatio", 1.0f);
				camera->FOV = (float)perspectiveCameraJSON->getFieldNumberOrDefault("yfov", 1.0f);
				camera->FOV = mathem::toDegrees(camera->FOV);
				camera->farCullPlane = (float)perspectiveCameraJSON->getFieldNumberOrDefault("zfar", 100.0f);
				camera->nearCullPlane = (float)perspectiveCameraJSON->getFieldNumberOrDefault("znear", 0.001f);
			}
			else if (cameraType == "orthographic")
			{
				camera->_type = render::CameraType::ORTHOGRAPHIC;
				JSON* orthographicCameraJSON = cameraJSON->getFieldObject("orthographic");

				double xMagnification = orthographicCameraJSON->getFieldNumberOrDefault("xmag", 1.0f);
				double yMagnification = orthographicCameraJSON->getFieldNumberOrDefault("ymag", 1.0f);
				camera->farCullPlane = (float)orthographicCameraJSON->getFieldNumberOrDefault("zfar", 100.0f);
				camera->nearCullPlane = (float)orthographicCameraJSON->getFieldNumberOrDefault("znear", 0.001f);
			}

			asset3DResult->cameras.setElement(i, camera);
		}
	}

	void collectBuffers(mitevox::Asset3D* asset3DResult, fileio::JSON* buffersArrayJSON, std::string filePath)
	{
		if (buffersArrayJSON == nullptr)
		{
			return;
		}

		int64_t buffersCount = buffersArrayJSON->getArraySize();
		asset3DResult->buffers.resize(buffersCount);
		asset3DResult->buffers.setAllElementsZeros();

		for (int64_t i = 0; i < buffersCount; ++i)
		{
			JSON* bufferJSON = buffersArrayJSON->getArrayItem(i);

			size_t byteLength = (size_t)bufferJSON->getFieldNumberOrDefault("byteLength", 0.0f);
			safety::SafeByteArray* buffer = new safety::SafeByteArray();

			std::string uri = bufferJSON->getFieldStringOrDefault("uri", "ERROR");
			if (uri != "ERROR")
			{
				if (fs::path(uri).is_absolute() == false)
				{
					uri = fs::path(filePath).parent_path().string() + "\\" + uri;
				}
				fileio::FileStatus fileStatus =
					fileio::FileInputOutput::loadBinary(uri, buffer);
			}

			asset3DResult->buffers.setElement(i, buffer);
		}
	}

	void collectBufferViews(mitevox::Asset3D* asset3DResult, fileio::JSON* bufferViewsArrayJSON)
	{
		if (bufferViewsArrayJSON == nullptr)
		{
			return;
		}

		int64_t bufferViewsCount = bufferViewsArrayJSON->getArraySize();
		asset3DResult->bufferViews.resize(bufferViewsCount);
		asset3DResult->bufferViews.setAllElementsZeros();

		for (int64_t i = 0; i < bufferViewsCount; ++i)
		{
			JSON* bufferViewJSON = bufferViewsArrayJSON->getArrayItem(i);
			mitevox::BufferView* bufferView = new mitevox::BufferView();
			BufferViewCodec::fromGLTF(bufferView, bufferViewJSON, &asset3DResult->buffers);
			asset3DResult->bufferViews.setElement(i, bufferView);
		}
	}

	void collectAccessors(mitevox::Asset3D* asset3DResult, fileio::JSON* accessorsArrayJSON)
	{
		if (accessorsArrayJSON == nullptr)
		{
			return;
		}

		int64_t accessorsCount = accessorsArrayJSON->getArraySize();
		asset3DResult->accessors.resize(accessorsCount);
		asset3DResult->accessors.setAllElementsZeros();

		for (int64_t i = 0; i < accessorsCount; ++i)
		{
			JSON* accessorJSON = accessorsArrayJSON->getArrayItem(i);
			mitevox::BufferViewAccessor* accessor = new mitevox::BufferViewAccessor();
			BufferViewAccessorCodec::fromGLTF(accessor, accessorJSON, &asset3DResult->bufferViews);
			asset3DResult->accessors.setElement(i, accessor);
		}
	}

	void collectImageSamplers(mitevox::Asset3D* asset3DResult, fileio::JSON* imageSamplersArrayJSON)
	{
		if (imageSamplersArrayJSON == nullptr)
		{
			return;
		}

		int64_t imageSamplersCount = imageSamplersArrayJSON->getArraySize();
		asset3DResult->imageSamplers.resize(imageSamplersCount);

		for (int64_t i = 0; i < imageSamplersCount; ++i)
		{
			JSON* imageSamplerJSON = imageSamplersArrayJSON->getArrayItem(i);
			mitevox::ImageSampler imageSampler;
			ImageSamplerCodec::fromGLTF(&imageSampler, imageSamplerJSON);
			asset3DResult->imageSamplers.setElement(i, imageSampler);
		}
	}

	void collectImages(mitevox::Asset3D* asset3DResult, fileio::JSON* imagesArrayJSON, std::string filePath)
	{
		if (imagesArrayJSON == nullptr)
		{
			return;
		}

		int64_t imagesCount = imagesArrayJSON->getArraySize();
		asset3DResult->images.resize(imagesCount);

		for (int64_t i = 0; i < imagesCount; ++i)
		{
			JSON* imageJSON = imagesArrayJSON->getArrayItem(i);
			asset3DResult->images.setElement(i, mitevox::Image());
			mitevox::Image* image = asset3DResult->images.getElementPointer(i);
			ImageCodec::fromGLTF(image, imageJSON, filePath);
		}
	}

	void collectTextures(mitevox::Asset3D* asset3DResult, fileio::JSON* texturesArrayJSON)
	{
		if (texturesArrayJSON == nullptr)
		{
			return;
		}

		int64_t texturesCount = texturesArrayJSON->getArraySize();
		asset3DResult->textures.resize(texturesCount);

		for (int64_t i = 0; i < texturesCount; ++i)
		{
			JSON* textureJSON = texturesArrayJSON->getArrayItem(i);

			asset3DResult->textures.setElement(i, mitevox::Texture());
			mitevox::Texture* texture = asset3DResult->textures.getElementPointer(i);

			texture->name = textureJSON->getFieldStringOrDefault("name", "Untitled");
			if (JSON* numberJSON = textureJSON->getField("sampler"))
			{
				int32_t samplerIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
				texture->sampler = asset3DResult->imageSamplers.getElementPointer(samplerIndex);
			}

			if (JSON* numberJSON = textureJSON->getField("source"))
			{
				int32_t imageIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
				texture->image = asset3DResult->images.getElementPointer(imageIndex);
			}
		}
	}

	void collectMaterials(mitevox::Asset3D* asset3DResult, fileio::JSON* materialsArrayJSON)
	{
		if (materialsArrayJSON == nullptr)
		{
			return;
		}

		int64_t materialsCount = materialsArrayJSON->getArraySize();
		asset3DResult->materials.resize(materialsCount);

		for (int64_t i = 0; i < materialsCount; ++i)
		{
			JSON* materialJSON = materialsArrayJSON->getArrayItem(i);
			asset3DResult->materials.setElement(i, mitevox::Material());
			mitevox::Material* material = asset3DResult->materials.getElementPointer(i);
			MaterialCodec::fromGLTF(material, materialJSON, &asset3DResult->textures);
		}
	}

	void collectMeshes(mitevox::Asset3D* asset3DResult, fileio::JSON* meshesArrayJSON)
	{
		if (meshesArrayJSON == nullptr)
		{
			return;
		}

		size_t meshesCount = meshesArrayJSON->getArraySize();
		asset3DResult->meshes.resize(meshesCount);

		for (size_t i = 0; i < meshesCount; ++i)
		{
			JSON* meshJSON = meshesArrayJSON->getArrayItem(i);
			mitevox::Mesh* mesh = new mitevox::Mesh();
			MeshCodec::fromGLTF(mesh, meshJSON, &asset3DResult->accessors, &asset3DResult->materials);
			asset3DResult->meshes.setElement((int64_t)i, mesh);
		}
	}

	void collectNodes(mitevox::Asset3D* asset3DResult, fileio::JSON* nodesArrayJSON)
	{
		if (nodesArrayJSON == nullptr)
		{
			return;
		}

		size_t nodesCount = nodesArrayJSON->getArraySize();
		asset3DResult->nodes.resize(nodesCount);

		for (size_t i = 0; i < nodesCount; ++i)
		{
			asset3DResult->nodes.setElement((int64_t)i, new mitevox::Node());
		}

		for (size_t i = 0; i < nodesCount; ++i)
		{
			JSON* nodeJSON = nodesArrayJSON->getArrayItem(i);
			mitevox::Node* node = asset3DResult->nodes.getElement(i);
			NodeCodec::fromGLTF(
				node, 
				nodeJSON, 
				&asset3DResult->cameras, 
				&asset3DResult->meshes, 
				&asset3DResult->nodes);
		}
	}

	void collectScenes(mitevox::Asset3D* asset3DResult, fileio::JSON* scenesArrayJSON)
	{
		if (scenesArrayJSON == nullptr)
		{
			return;
		}

		size_t scenesCount = scenesArrayJSON->getArraySize();
		asset3DResult->scenes.resize(scenesCount);

		for (size_t i = 0; i < scenesCount; ++i)
		{
			JSON* sceneJSON = scenesArrayJSON->getArrayItem(i);
			mitevox::Scene* scene = new mitevox::Scene();
			SceneCodec::fromGLTF(scene, sceneJSON, &asset3DResult->nodes);
			asset3DResult->scenes.setElement((int64_t)i, scene);

			if (scene->activeCameraEntity == nullptr)
			{
				size_t entitiesCount = scene->entities.getElementsCount();
				for (size_t i = 0; i < entitiesCount; ++i)
				{
					mitevox::Entity* entity = scene->entities.getElement(i);
					if (entity->hasCamera())
					{
						scene->activeCameraEntity = entity;
						break;
					}
				}
			}
		}
	}

	void collectAnimations(mitevox::Asset3D* asset3DResult, JSON* animationsArrayJSON)
	{
		if (animationsArrayJSON == nullptr)
		{
			return;
		}

		size_t animationsCount = animationsArrayJSON->getArraySize();
		asset3DResult->animations.resize(animationsCount);

		for (size_t i = 0; i < animationsCount; ++i)
		{
			JSON* animationJSON = animationsArrayJSON->getArrayItem(i);
			mitevox::Animation* animation = new mitevox::Animation();
			AnimationCodec::fromGLTF(
				animation, animationJSON, 
				&asset3DResult->nodes, 
				&asset3DResult->accessors);
			asset3DResult->animations.setElement((int64_t)i, animation);
		}
	}

	void collectSkins(mitevox::Asset3D* asset3DResult, JSON* skinsArrayJSON)
	{
		if (skinsArrayJSON == nullptr)
		{
			return;
		}

		size_t skinsCount = skinsArrayJSON->getArraySize();
		asset3DResult->skeletons.resize(skinsCount);

		for (size_t i = 0; i < skinsCount; ++i)
		{
			JSON* skinJSON = skinsArrayJSON->getArrayItem(i);
			mitevox::NodeBasedSkeleton* skeleton = new mitevox::NodeBasedSkeleton();
			NodeBasedSkeletonCodec::fromGLTF(
				skeleton, skinJSON,
				&asset3DResult->nodes,
				&asset3DResult->accessors);
			asset3DResult->skeletons.setElement((int64_t)i, skeleton);
		}
	}

	void resolveSkeletonsPointers(mitevox::Asset3D* asset3DResult)
	{
		size_t nodesCount = asset3DResult->nodes.getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mitevox::Node* node = asset3DResult->nodes.getElement(i);
			if (node->skeleton != 0)
			{
				node->skeleton = asset3DResult->skeletons.getElement((size_t)node->skeleton - 1);
			}
		}
	}

	void preparePlayground(mitevox::Asset3D* asset3DResult)
	{
		// Generate tangents if needed
		size_t meshesCount = asset3DResult->meshes.getElementsCount();
		for (size_t i = 0; i < meshesCount; ++i)
		{
			mitevox::Mesh* mesh = asset3DResult->meshes.getElement(i);
			// TODO: mesh->tryGenerateTangents();
		}
	}

	void saveExtensions(JSON* glTFJSON, mitevox::Asset3D* asset3D)
	{

	}

	void saveCameras(mitevox::Asset3D* asset3D, JSON* camerasArrayJSONResult)
	{
		size_t camerasCount = asset3D->cameras.getElementsCount();
		for (size_t i = 0; i < camerasCount; ++i)
		{
			render::Camera* camera = asset3D->cameras.getElement(i);

			size_t cameraJSONIndex = camerasArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* cameraJSON = camerasArrayJSONResult->getArrayItem(cameraJSONIndex);
			cameraJSON->setField("name", camera->_name);

			if (camera->_type == render::CameraType::PERPECTIVE)
			{
				cameraJSON->setField("type", std::string("perspective"));
				JSON* perspectiveCameraJSON = cameraJSON->setFieldType("perspective", JSONtype::OBJECT);
				perspectiveCameraJSON->setField("aspectRatio", camera->aspectRatio);
				perspectiveCameraJSON->setField("yfov", mathem::toRadians(camera->FOV));
				perspectiveCameraJSON->setField("zfar", camera->farCullPlane);
				perspectiveCameraJSON->setField("znear", camera->nearCullPlane);
			}
			else if (camera->_type == render::CameraType::ORTHOGRAPHIC)
			{
				cameraJSON->setField("type", std::string("orthographic"));
				JSON* orthographicCameraJSON = cameraJSON->setFieldType("orthographic", JSONtype::OBJECT);
				// TODO: orthographicCameraJSON->setField("xmag", 1.0f);
				// TODO: orthographicCameraJSON->setField("ymag", 1.0f);
				orthographicCameraJSON->setField("zfar", camera->farCullPlane);
				orthographicCameraJSON->setField("znear", camera->nearCullPlane);
			}
		}
	}

	void saveBuffers(mitevox::Asset3D* asset3D, JSON* buffersArrayJSONResult, std::string filePath)
	{
		size_t buffersCount = asset3D->buffers.getElementsCount();
		for (size_t i = 0; i < buffersCount; ++i)
		{
			safety::SafeByteArray* buffer = asset3D->buffers.getElement(i);

			size_t bufferJSONIndex = buffersArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* bufferJSON = buffersArrayJSONResult->getArrayItem(bufferJSONIndex);
			std::string bufferName = "buffer" + std::to_string(i);
			bufferJSON->setField("name", bufferName);
			bufferName += ".bin";
			bufferJSON->setField("uri", bufferName);
			bufferJSON->setField("byteLength", (double)buffer->getElementsCount());

			fileio::FileStatus fileStatus = fileio::FileInputOutput::saveBinary(buffer, bufferName); // TODO: 
		}
	}

	void saveBufferViews(mitevox::Asset3D* asset3D, JSON* bufferViewsArrayJSONResult)
	{
		size_t bufferViewsCount = asset3D->bufferViews.getElementsCount();
		for (size_t i = 0; i < bufferViewsCount; ++i)
		{
			mitevox::BufferView* bufferView = asset3D->bufferViews.getElement(i);

			size_t bufferViewJSONIndex = bufferViewsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* bufferViewJSON = bufferViewsArrayJSONResult->getArrayItem(bufferViewJSONIndex);
			BufferViewCodec::toGLTF(bufferViewJSON, bufferView, &asset3D->buffers);
		}
	}

	void saveAccessors(mitevox::Asset3D* asset3D, JSON* accessorsArrayJSONResult)
	{
		size_t accessorsCount = asset3D->accessors.getElementsCount();
		for (size_t i = 0; i < accessorsCount; ++i)
		{
			mitevox::BufferViewAccessor* accessor = asset3D->accessors.getElement(i);

			size_t accessorJSONIndex = accessorsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* accessorJSON = accessorsArrayJSONResult->getArrayItem(accessorJSONIndex);
			BufferViewAccessorCodec::toGLTF(accessorJSON, accessor, &asset3D->bufferViews);
		}
	}

	void saveImageSamplers(mitevox::Asset3D* asset3D, JSON* imageSamplersArrayJSONResult)
	{
		size_t imageSamplersCount = asset3D->imageSamplers.getElementsCount();
		for (size_t i = 0; i < imageSamplersCount; ++i)
		{
			mitevox::ImageSampler imageSampler = asset3D->imageSamplers.getElement(i);

			size_t imageSamplerJSONIndex = imageSamplersArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* imageSamplerJSON = imageSamplersArrayJSONResult->getArrayItem(imageSamplerJSONIndex);
			ImageSamplerCodec::toGLTF(imageSamplerJSON, &imageSampler);
		}
	}

	void saveImages(mitevox::Asset3D* asset3D, JSON* imagesArrayJSONResult, std::string filePath)
	{
		size_t imagesCount = asset3D->images.getElementsCount();
		for (size_t i = 0; i < imagesCount; ++i)
		{
			mitevox::Image* image = asset3D->images.getElementPointer(i);

			size_t imageJSONIndex = imagesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* imageJSON = imagesArrayJSONResult->getArrayItem(imageJSONIndex);
			ImageCodec::toGLTF(imageJSON, image, i, filePath);
		}
	}

	void saveTextures(mitevox::Asset3D* asset3D, JSON* texturesArrayJSONResult)
	{
		size_t texturesCount = asset3D->textures.getElementsCount();
		for (size_t i = 0; i < texturesCount; ++i)
		{
			mitevox::Texture* texture = asset3D->textures.getElementPointer(i);

			size_t textureJSONIndex = texturesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* textureJSON = texturesArrayJSONResult->getArrayItem(textureJSONIndex);
			textureJSON->setField("name", texture->name);

			if (texture->sampler)
			{
				size_t imageSamplersCount = asset3D->imageSamplers.getElementsCount();
				for (size_t i = 0; i < imageSamplersCount; ++i)
				{
					if (texture->sampler == asset3D->imageSamplers.getElementPointer(i))
					{
						textureJSON->setField("sampler", (double)i);
						break;
					}
				}
			}

			if (texture->image)
			{
				size_t imagesCount = asset3D->images.getElementsCount();
				for (size_t i = 0; i < imagesCount; ++i)
				{
					if (texture->image == asset3D->images.getElementPointer(i))
					{
						textureJSON->setField("source", (double)i);
						break;
					}
				}
			}
		}
	}

	void saveMaterials(mitevox::Asset3D* asset3D, JSON* materialsArrayJSONResult)
	{
		size_t materialsCount = asset3D->materials.getElementsCount();
		for (size_t i = 0; i < materialsCount; ++i)
		{
			mitevox::Material* material = asset3D->materials.getElementPointer(i);

			size_t materialJSONIndex = materialsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* materialJSON = materialsArrayJSONResult->getArrayItem(materialJSONIndex);
			MaterialCodec::toGLTF(materialJSON, material, &asset3D->textures);
		}
	}

	void saveMeshes(mitevox::Asset3D* asset3D, JSON* meshesArrayJSONResult)
	{
		size_t meshesCount = asset3D->meshes.getElementsCount();
		for (size_t i = 0; i < meshesCount; ++i)
		{
			mitevox::Mesh* mesh = asset3D->meshes.getElement(i);

			size_t meshJSONIndex = meshesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* meshJSON = meshesArrayJSONResult->getArrayItem(meshJSONIndex);
			MeshCodec::toGLTF(meshJSON, mesh, &asset3D->accessors, &asset3D->materials);
		}
	}

	void saveNodes(mitevox::Asset3D* asset3D, JSON* nodesArrayJSONResult)
	{
		size_t nodesCount = asset3D->nodes.getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mitevox::Node* node = asset3D->nodes.getElement(i);

			size_t nodeJSONIndex = nodesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* nodeJSON = nodesArrayJSONResult->getArrayItem(nodeJSONIndex);
			NodeCodec::toGLTF(
				nodeJSON,
				node,
				&asset3D->cameras,
				&asset3D->meshes,
				&asset3D->nodes,
				&asset3D->skeletons);
		}
	}

	void saveScenes(mitevox::Asset3D* asset3D, JSON* scenesArrayJSONResult)
	{
		size_t scenesCount = asset3D->scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			mitevox::Scene* scene = asset3D->scenes.getElement(i);

			size_t sceneJSONIndex = scenesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* sceneJSON = scenesArrayJSONResult->getArrayItem(sceneJSONIndex);
			SceneCodec::toGLTF(sceneJSON, scene, &asset3D->nodes);
		}
	}

	void saveAnimations(mitevox::Asset3D* asset3D, JSON* animationsArrayJSONResult)
	{
		/*size_t animationsCount = animationsArrayJSON->getArraySize();
		asset3DResult->animations.resize(animationsCount);

		for (size_t i = 0; i < animationsCount; ++i)
		{
			JSON* animationJSON = animationsArrayJSON->getArrayItem(i);
			mitevox::Animation* animation = new mitevox::Animation();
			AnimationCodec::fromGLTF(
				animation, animationJSON,
				&asset3DResult->nodes,
				&asset3DResult->accessors);
			asset3DResult->animations.setElement((int64_t)i, animation);
		}*/
	}

	void saveSkins(mitevox::Asset3D* asset3D, JSON* skinsArrayJSONResult)
	{
		size_t skinsCount = asset3D->skeletons.getElementsCount();
		for (size_t i = 0; i < skinsCount; ++i)
		{
			mitevox::NodeBasedSkeleton* skeleton = (mitevox::NodeBasedSkeleton*)asset3D->skeletons.getElement(i);

			size_t skinJSONIndex = skinsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* skinJSON = skinsArrayJSONResult->getArrayItem(skinJSONIndex);
			NodeBasedSkeletonCodec::toGLTF(
				skinJSON,
				skeleton, 
				&asset3D->nodes,
				&asset3D->accessors);
		}
	}
}