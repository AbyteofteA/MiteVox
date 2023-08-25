#include "PlaygroundCodecGLTF.h"

#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/Formats/CodecGLTF/BufferLayoutCodec/BufferViewCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/BufferLayoutCodec/BufferViewAccessorCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/MaterialCodec/ImageCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/MaterialCodec/ImageSamplerCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/MaterialCodec/MaterialCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/MeshCodec/MeshCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/PlaygroundCodec/NodeCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/PlaygroundCodec/SceneCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/AnimationCodec/AnimationCodec.h"
#include "engine/FileIO/src/Formats/CodecGLTF/SkeletonCodec/NodeBasedSkeletonCodec.h"
#include "engine/Math/src/Utilities/Convertations.h"

#include <string>

namespace fileio
{
	DeserializationStatus PlaygroundCodecGLTF::fromGLTF(mitevox::Playground* playgroundResult, JSON* playgroundJSON, std::string filePath)
	{
		playgroundResult->file = filePath;

		JSON* assetJSON = playgroundJSON->getFieldObject("asset");
		playgroundResult->assetInfo.version = assetJSON->getFieldStringOrDefault("version", "");
		playgroundResult->assetInfo.minVersion = assetJSON->getFieldStringOrDefault("minVersion", "");
		playgroundResult->assetInfo.generator = assetJSON->getFieldStringOrDefault("generator", "");
		playgroundResult->assetInfo.copyright = assetJSON->getFieldStringOrDefault("copyright", "");

		collectExtensions(playgroundResult, playgroundJSON);
		if (extensionsAreSupported(playgroundResult) == false)
		{
			return DeserializationStatus::ERROR_UNKNOWN;
		}

		if (JSON* numberJSON = playgroundJSON->getField("scene"))
		{
			playgroundResult->activeScene = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
		}

		JSON* camerasArrayJSON = playgroundJSON->getFieldArray("cameras");
		collectCameras(playgroundResult, camerasArrayJSON);
		JSON* buffersArrayJSON = playgroundJSON->getFieldArray("buffers");
		collectBuffers(playgroundResult, buffersArrayJSON, filePath);
		JSON* bufferViewsArrayJSON = playgroundJSON->getFieldArray("bufferViews");
		collectBufferViews(playgroundResult, bufferViewsArrayJSON);
		JSON* accessorsArrayJSON = playgroundJSON->getFieldArray("accessors");
		collectAccessors(playgroundResult, accessorsArrayJSON);
		JSON* imageSamplerJSON = playgroundJSON->getFieldArray("samplers");
		collectImageSamplers(playgroundResult, imageSamplerJSON);
		JSON* imagesJSON = playgroundJSON->getFieldArray("images");
		collectImages(playgroundResult, imagesJSON, filePath);
		JSON* texturesJSON = playgroundJSON->getFieldArray("textures");
		collectTextures(playgroundResult, texturesJSON);
		JSON* materialsJSON = playgroundJSON->getFieldArray("materials");
		collectMaterials(playgroundResult, materialsJSON);
		JSON* meshesArrayJSON = playgroundJSON->getFieldArray("meshes");
		collectMeshes(playgroundResult, meshesArrayJSON);
		JSON* nodesArrayJSON = playgroundJSON->getFieldArray("nodes");
		collectNodes(playgroundResult, nodesArrayJSON);
		JSON* skinsArrayJSON = playgroundJSON->getFieldArray("skins");
		collectSkins(playgroundResult, skinsArrayJSON);
		resolveSkeletonsPointers(playgroundResult);
		JSON* scenesArrayJSON = playgroundJSON->getFieldArray("scenes");
		collectScenes(playgroundResult, scenesArrayJSON);
		JSON* animationsArrayJSON = playgroundJSON->getFieldArray("animations");
		collectAnimations(playgroundResult, animationsArrayJSON);

		preparePlayground(playgroundResult);

		return DeserializationStatus::COMPLETE;
	}

	SerializationStatus PlaygroundCodecGLTF::toGLTF(mitevox::Playground* playground, JSON* playgroundJSONResult, std::string filePath)
	{
		JSON* assetJSON = playgroundJSONResult->setFieldType("asset", JSONtype::OBJECT);
		assetJSON->setField("version", std::string("2.0"));
		//assetJSON->setField("generator", std::string("MiteVox"));

		saveExtensions(playgroundJSONResult, playground);

		if (playground->activeScene >= 0)
		{
			playgroundJSONResult->setField("scene", (double)playground->activeScene);
		}

		if (playground->cameras.getElementsCount() > 0)
		{
			JSON* camerasArrayJSONResult = playgroundJSONResult->setFieldType("cameras", JSONtype::ARRAY);
			saveCameras(playground, camerasArrayJSONResult);
		}
		if (playground->buffers.getElementsCount() > 0)
		{
			JSON* buffersArrayJSONResult = playgroundJSONResult->setFieldType("buffers", JSONtype::ARRAY);
			saveBuffers(playground, buffersArrayJSONResult, filePath);
		}
		if (playground->bufferViews.getElementsCount() > 0)
		{
			JSON* bufferViewsArrayJSONResult = playgroundJSONResult->setFieldType("bufferViews", JSONtype::ARRAY);
			saveBufferViews(playground, bufferViewsArrayJSONResult);
		}
		if (playground->accessors.getElementsCount() > 0)
		{
			JSON* accessorsArrayJSONResult = playgroundJSONResult->setFieldType("accessors", JSONtype::ARRAY);
			saveAccessors(playground, accessorsArrayJSONResult);
		}
		if (playground->imageSamplers.getElementsCount() > 0)
		{
			JSON* imageSamplersArrayJSONResult = playgroundJSONResult->setFieldType("samplers", JSONtype::ARRAY);
			saveImageSamplers(playground, imageSamplersArrayJSONResult);
		}
		if (playground->images.getElementsCount() > 0)
		{
			JSON* imagesArrayJSONResult = playgroundJSONResult->setFieldType("images", JSONtype::ARRAY);
			saveImages(playground, imagesArrayJSONResult, filePath);
		}
		if (playground->textures.getElementsCount() > 0)
		{
			JSON* texturesArrayJSONResult = playgroundJSONResult->setFieldType("textures", JSONtype::ARRAY);
			saveTextures(playground, texturesArrayJSONResult);
		}
		if (playground->materials.getElementsCount() > 0)
		{
			JSON* materialsArrayJSONResult = playgroundJSONResult->setFieldType("materials", JSONtype::ARRAY);
			saveMaterials(playground, materialsArrayJSONResult);
		}
		if (playground->meshes.getElementsCount() > 0)
		{
			JSON* meshesArrayJSONResult = playgroundJSONResult->setFieldType("meshes", JSONtype::ARRAY);
			saveMeshes(playground, meshesArrayJSONResult);
		}
		if (playground->nodes.getElementsCount() > 0)
		{
			JSON* nodesArrayJSONResult = playgroundJSONResult->setFieldType("nodes", JSONtype::ARRAY);
			saveNodes(playground, nodesArrayJSONResult);
		}
		if (playground->skeletons.getElementsCount() > 0)
		{
			JSON* skinsArrayJSONResult = playgroundJSONResult->setFieldType("skins", JSONtype::ARRAY);
			saveSkins(playground, skinsArrayJSONResult);
		}
		if (playground->scenes.getElementsCount() > 0)
		{
			JSON* scenesArrayJSONResult = playgroundJSONResult->setFieldType("scenes", JSONtype::ARRAY);
			saveScenes(playground, scenesArrayJSONResult);
		}
		if (playground->animations.getElementsCount() > 0)
		{
			JSON* animationsArrayJSONResult = playgroundJSONResult->setFieldType("animations", JSONtype::ARRAY);
			saveAnimations(playground, animationsArrayJSONResult);
		}

		return SerializationStatus::COMPLETE;
	}

	void PlaygroundCodecGLTF::collectExtensions(mitevox::Playground* playground, fileio::JSON* glTFJSON)
	{
		if (glTFJSON == nullptr)
		{
			return;
		}

		fileio::JSON* extensionsUsedJSON = glTFJSON->getFieldArray("extensionsUsed");
		if (extensionsUsedJSON != nullptr)
		{
			extensionsUsedJSON->toStringArrayOrDefault(&playground->extensionsUsed, "ERROR");
		}

		fileio::JSON* extensionsRequiredJSON = glTFJSON->getFieldArray("extensionsRequired");
		if (extensionsRequiredJSON != nullptr)
		{
			extensionsRequiredJSON->toStringArrayOrDefault(&playground->extensionsRequired, "ERROR");
		}
	}

	bool PlaygroundCodecGLTF::extensionsAreSupported(mitevox::Playground* playground)
	{
		int64_t extensionsUsedCount = playground->extensionsUsed.getElementsCount();
		int64_t extensionsRequiredCount = playground->extensionsRequired.getElementsCount();
		bool extensionIsSupported = false;
		for (int64_t indexRequired = 0; indexRequired < extensionsRequiredCount; ++indexRequired)
		{
			extensionIsSupported = false;
			std::string extensionRequired = playground->extensionsRequired.getElement(indexRequired);
			for (int64_t indexUsed = 0; indexUsed < extensionsUsedCount; ++indexUsed)
			{
				std::string extensionUsed = playground->extensionsUsed.getElement(indexUsed);
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

	void PlaygroundCodecGLTF::collectCameras(mitevox::Playground* playgroundResult, fileio::JSON* camerasArrayJSON)
	{
		if (camerasArrayJSON == nullptr)
		{
			return;
		}

		int64_t camerasCount = camerasArrayJSON->getArraySize();
		playgroundResult->cameras.resize(camerasCount);

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

			playgroundResult->cameras.setElement(i, camera);
		}
	}

	void PlaygroundCodecGLTF::collectBuffers(mitevox::Playground* playgroundResult, fileio::JSON* buffersArrayJSON, std::string filePath)
	{
		if (buffersArrayJSON == nullptr)
		{
			return;
		}

		int64_t buffersCount = buffersArrayJSON->getArraySize();
		playgroundResult->buffers.resize(buffersCount);
		playgroundResult->buffers.setAllElementsZeros();

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

			playgroundResult->buffers.setElement(i, buffer);
		}
	}

	void PlaygroundCodecGLTF::collectBufferViews(mitevox::Playground* playgroundResult, fileio::JSON* bufferViewsArrayJSON)
	{
		if (bufferViewsArrayJSON == nullptr)
		{
			return;
		}

		int64_t bufferViewsCount = bufferViewsArrayJSON->getArraySize();
		playgroundResult->bufferViews.resize(bufferViewsCount);
		playgroundResult->bufferViews.setAllElementsZeros();

		for (int64_t i = 0; i < bufferViewsCount; ++i)
		{
			JSON* bufferViewJSON = bufferViewsArrayJSON->getArrayItem(i);
			mitevox::BufferView* bufferView = new mitevox::BufferView();
			BufferViewCodec::fromGLTF(bufferView, bufferViewJSON, &playgroundResult->buffers);
			playgroundResult->bufferViews.setElement(i, bufferView);
		}
	}

	void PlaygroundCodecGLTF::collectAccessors(mitevox::Playground* playgroundResult, fileio::JSON* accessorsArrayJSON)
	{
		if (accessorsArrayJSON == nullptr)
		{
			return;
		}

		int64_t accessorsCount = accessorsArrayJSON->getArraySize();
		playgroundResult->accessors.resize(accessorsCount);
		playgroundResult->accessors.setAllElementsZeros();

		for (int64_t i = 0; i < accessorsCount; ++i)
		{
			JSON* accessorJSON = accessorsArrayJSON->getArrayItem(i);
			mitevox::BufferViewAccessor* accessor = new mitevox::BufferViewAccessor();
			BufferViewAccessorCodec::fromGLTF(accessor, accessorJSON, &playgroundResult->bufferViews);
			playgroundResult->accessors.setElement(i, accessor);
		}
	}

	void PlaygroundCodecGLTF::collectImageSamplers(mitevox::Playground* playgroundResult, fileio::JSON* imageSamplersArrayJSON)
	{
		if (imageSamplersArrayJSON == nullptr)
		{
			return;
		}

		int64_t imageSamplersCount = imageSamplersArrayJSON->getArraySize();
		playgroundResult->imageSamplers.resize(imageSamplersCount);

		for (int64_t i = 0; i < imageSamplersCount; ++i)
		{
			JSON* imageSamplerJSON = imageSamplersArrayJSON->getArrayItem(i);
			mitevox::ImageSampler imageSampler;
			ImageSamplerCodec::fromGLTF(&imageSampler, imageSamplerJSON);
			playgroundResult->imageSamplers.setElement(i, imageSampler);
		}
	}

	void PlaygroundCodecGLTF::collectImages(mitevox::Playground* playgroundResult, fileio::JSON* imagesArrayJSON, std::string filePath)
	{
		if (imagesArrayJSON == nullptr)
		{
			return;
		}

		int64_t imagesCount = imagesArrayJSON->getArraySize();
		playgroundResult->images.resize(imagesCount);

		for (int64_t i = 0; i < imagesCount; ++i)
		{
			JSON* imageJSON = imagesArrayJSON->getArrayItem(i);
			playgroundResult->images.setElement(i, mitevox::Image());
			mitevox::Image* image = playgroundResult->images.getElementPointer(i);
			ImageCodec::fromGLTF(image, imageJSON, filePath);
		}
	}

	void PlaygroundCodecGLTF::collectTextures(mitevox::Playground* playgroundResult, fileio::JSON* texturesArrayJSON)
	{
		if (texturesArrayJSON == nullptr)
		{
			return;
		}

		int64_t texturesCount = texturesArrayJSON->getArraySize();
		playgroundResult->textures.resize(texturesCount);

		for (int64_t i = 0; i < texturesCount; ++i)
		{
			JSON* textureJSON = texturesArrayJSON->getArrayItem(i);

			playgroundResult->textures.setElement(i, mitevox::Texture());
			mitevox::Texture* texture = playgroundResult->textures.getElementPointer(i);

			texture->name = textureJSON->getFieldStringOrDefault("name", "Untitled");
			if (JSON* numberJSON = textureJSON->getField("sampler"))
			{
				int32_t samplerIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
				texture->sampler = playgroundResult->imageSamplers.getElementPointer(samplerIndex);
			}

			if (JSON* numberJSON = textureJSON->getField("source"))
			{
				int32_t imageIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
				texture->image = playgroundResult->images.getElementPointer(imageIndex);
			}
		}
	}

	void PlaygroundCodecGLTF::collectMaterials(mitevox::Playground* playgroundResult, fileio::JSON* materialsArrayJSON)
	{
		if (materialsArrayJSON == nullptr)
		{
			return;
		}

		int64_t materialsCount = materialsArrayJSON->getArraySize();
		playgroundResult->materials.resize(materialsCount);

		for (int64_t i = 0; i < materialsCount; ++i)
		{
			JSON* materialJSON = materialsArrayJSON->getArrayItem(i);
			playgroundResult->materials.setElement(i, mitevox::Material());
			mitevox::Material* material = playgroundResult->materials.getElementPointer(i);
			MaterialCodec::fromGLTF(material, materialJSON, &playgroundResult->textures);
		}
	}

	void PlaygroundCodecGLTF::collectMeshes(mitevox::Playground* playgroundResult, fileio::JSON* meshesArrayJSON)
	{
		if (meshesArrayJSON == nullptr)
		{
			return;
		}

		size_t meshesCount = meshesArrayJSON->getArraySize();
		playgroundResult->meshes.resize(meshesCount);

		for (size_t i = 0; i < meshesCount; ++i)
		{
			JSON* meshJSON = meshesArrayJSON->getArrayItem(i);
			mitevox::Mesh* mesh = new mitevox::Mesh();
			MeshCodec::fromGLTF(mesh, meshJSON, &playgroundResult->accessors, &playgroundResult->materials);
			playgroundResult->meshes.setElement((int64_t)i, mesh);
		}
	}

	void PlaygroundCodecGLTF::collectNodes(mitevox::Playground* playgroundResult, fileio::JSON* nodesArrayJSON)
	{
		if (nodesArrayJSON == nullptr)
		{
			return;
		}

		size_t nodesCount = nodesArrayJSON->getArraySize();
		playgroundResult->nodes.resize(nodesCount);

		for (size_t i = 0; i < nodesCount; ++i)
		{
			playgroundResult->nodes.setElement((int64_t)i, new mitevox::Node());
		}

		for (size_t i = 0; i < nodesCount; ++i)
		{
			JSON* nodeJSON = nodesArrayJSON->getArrayItem(i);
			mitevox::Node* node = playgroundResult->nodes.getElement(i);
			NodeCodec::fromGLTF(
				node, 
				nodeJSON, 
				&playgroundResult->cameras, 
				&playgroundResult->meshes, 
				&playgroundResult->nodes);
		}
	}

	void PlaygroundCodecGLTF::collectScenes(mitevox::Playground* playgroundResult, fileio::JSON* scenesArrayJSON)
	{
		if (scenesArrayJSON == nullptr)
		{
			return;
		}

		size_t scenesCount = scenesArrayJSON->getArraySize();
		playgroundResult->scenes.resize(scenesCount);

		for (size_t i = 0; i < scenesCount; ++i)
		{
			JSON* sceneJSON = scenesArrayJSON->getArrayItem(i);
			mitevox::Scene* scene = new mitevox::Scene();
			SceneCodec::fromGLTF(scene, sceneJSON, &playgroundResult->nodes);
			playgroundResult->scenes.setElement((int64_t)i, scene);

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

	void PlaygroundCodecGLTF::collectAnimations(mitevox::Playground* playgroundResult, JSON* animationsArrayJSON)
	{
		if (animationsArrayJSON == nullptr)
		{
			return;
		}

		size_t animationsCount = animationsArrayJSON->getArraySize();
		playgroundResult->animations.resize(animationsCount);

		for (size_t i = 0; i < animationsCount; ++i)
		{
			JSON* animationJSON = animationsArrayJSON->getArrayItem(i);
			mitevox::Animation* animation = new mitevox::Animation();
			AnimationCodec::fromGLTF(
				animation, animationJSON, 
				&playgroundResult->nodes, 
				&playgroundResult->accessors);
			playgroundResult->animations.setElement((int64_t)i, animation);
		}
	}

	void PlaygroundCodecGLTF::collectSkins(mitevox::Playground* playgroundResult, JSON* skinsArrayJSON)
	{
		if (skinsArrayJSON == nullptr)
		{
			return;
		}

		size_t skinsCount = skinsArrayJSON->getArraySize();
		playgroundResult->skeletons.resize(skinsCount);

		for (size_t i = 0; i < skinsCount; ++i)
		{
			JSON* skinJSON = skinsArrayJSON->getArrayItem(i);
			mitevox::NodeBasedSkeleton* skeleton = new mitevox::NodeBasedSkeleton();
			NodeBasedSkeletonCodec::fromGLTF(
				skeleton, skinJSON,
				&playgroundResult->nodes,
				&playgroundResult->accessors);
			playgroundResult->skeletons.setElement((int64_t)i, skeleton);
		}
	}

	void PlaygroundCodecGLTF::resolveSkeletonsPointers(mitevox::Playground* playgroundResult)
	{
		size_t nodesCount = playgroundResult->nodes.getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mitevox::Node* node = playgroundResult->nodes.getElement(i);
			if (node->skeleton != 0)
			{
				node->skeleton = playgroundResult->skeletons.getElement((size_t)node->skeleton - 1);
			}
		}
	}

	void PlaygroundCodecGLTF::preparePlayground(mitevox::Playground* playgroundResult)
	{
		// Generate tangents if needed
		size_t meshesCount = playgroundResult->meshes.getElementsCount();
		for (size_t i = 0; i < meshesCount; ++i)
		{
			mitevox::Mesh* mesh = playgroundResult->meshes.getElement(i);
			// TODO: mesh->tryGenerateTangents();
		}
	}

	void PlaygroundCodecGLTF::saveExtensions(JSON* glTFJSON, mitevox::Playground* playground)
	{

	}

	void PlaygroundCodecGLTF::saveCameras(mitevox::Playground* playground, JSON* camerasArrayJSONResult)
	{
		size_t camerasCount = playground->cameras.getElementsCount();
		for (size_t i = 0; i < camerasCount; ++i)
		{
			render::Camera* camera = playground->cameras.getElement(i);

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

	void PlaygroundCodecGLTF::saveBuffers(mitevox::Playground* playground, JSON* buffersArrayJSONResult, std::string filePath)
	{
		size_t buffersCount = playground->buffers.getElementsCount();
		for (size_t i = 0; i < buffersCount; ++i)
		{
			safety::SafeByteArray* buffer = playground->buffers.getElement(i);

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

	void PlaygroundCodecGLTF::saveBufferViews(mitevox::Playground* playground, JSON* bufferViewsArrayJSONResult)
	{
		size_t bufferViewsCount = playground->bufferViews.getElementsCount();
		for (size_t i = 0; i < bufferViewsCount; ++i)
		{
			mitevox::BufferView* bufferView = playground->bufferViews.getElement(i);

			size_t bufferViewJSONIndex = bufferViewsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* bufferViewJSON = bufferViewsArrayJSONResult->getArrayItem(bufferViewJSONIndex);
			BufferViewCodec::toGLTF(bufferViewJSON, bufferView, &playground->buffers);
		}
	}

	void PlaygroundCodecGLTF::saveAccessors(mitevox::Playground* playground, JSON* accessorsArrayJSONResult)
	{
		size_t accessorsCount = playground->accessors.getElementsCount();
		for (size_t i = 0; i < accessorsCount; ++i)
		{
			mitevox::BufferViewAccessor* accessor = playground->accessors.getElement(i);

			size_t accessorJSONIndex = accessorsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* accessorJSON = accessorsArrayJSONResult->getArrayItem(accessorJSONIndex);
			BufferViewAccessorCodec::toGLTF(accessorJSON, accessor, &playground->bufferViews);
		}
	}

	void PlaygroundCodecGLTF::saveImageSamplers(mitevox::Playground* playground, JSON* imageSamplersArrayJSONResult)
	{
		size_t imageSamplersCount = playground->imageSamplers.getElementsCount();
		for (size_t i = 0; i < imageSamplersCount; ++i)
		{
			mitevox::ImageSampler imageSampler = playground->imageSamplers.getElement(i);

			size_t imageSamplerJSONIndex = imageSamplersArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* imageSamplerJSON = imageSamplersArrayJSONResult->getArrayItem(imageSamplerJSONIndex);
			ImageSamplerCodec::toGLTF(imageSamplerJSON, &imageSampler);
		}
	}

	void PlaygroundCodecGLTF::saveImages(mitevox::Playground* playground, JSON* imagesArrayJSONResult, std::string filePath)
	{
		size_t imagesCount = playground->images.getElementsCount();
		for (size_t i = 0; i < imagesCount; ++i)
		{
			mitevox::Image* image = playground->images.getElementPointer(i);

			size_t imageJSONIndex = imagesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* imageJSON = imagesArrayJSONResult->getArrayItem(imageJSONIndex);
			ImageCodec::toGLTF(imageJSON, image, i, filePath);
		}
	}

	void PlaygroundCodecGLTF::saveTextures(mitevox::Playground* playground, JSON* texturesArrayJSONResult)
	{
		size_t texturesCount = playground->textures.getElementsCount();
		for (size_t i = 0; i < texturesCount; ++i)
		{
			mitevox::Texture* texture = playground->textures.getElementPointer(i);

			size_t textureJSONIndex = texturesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* textureJSON = texturesArrayJSONResult->getArrayItem(textureJSONIndex);
			textureJSON->setField("name", texture->name);

			if (texture->sampler)
			{
				size_t imageSamplersCount = playground->imageSamplers.getElementsCount();
				for (size_t i = 0; i < imageSamplersCount; ++i)
				{
					if (texture->sampler == playground->imageSamplers.getElementPointer(i))
					{
						textureJSON->setField("sampler", (double)i);
						break;
					}
				}
			}

			if (texture->image)
			{
				size_t imagesCount = playground->images.getElementsCount();
				for (size_t i = 0; i < imagesCount; ++i)
				{
					if (texture->image == playground->images.getElementPointer(i))
					{
						textureJSON->setField("source", (double)i);
						break;
					}
				}
			}
		}
	}

	void PlaygroundCodecGLTF::saveMaterials(mitevox::Playground* playground, JSON* materialsArrayJSONResult)
	{
		size_t materialsCount = playground->materials.getElementsCount();
		for (size_t i = 0; i < materialsCount; ++i)
		{
			mitevox::Material* material = playground->materials.getElementPointer(i);

			size_t materialJSONIndex = materialsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* materialJSON = materialsArrayJSONResult->getArrayItem(materialJSONIndex);
			MaterialCodec::toGLTF(materialJSON, material, &playground->textures);
		}
	}

	void PlaygroundCodecGLTF::saveMeshes(mitevox::Playground* playground, JSON* meshesArrayJSONResult)
	{
		size_t meshesCount = playground->meshes.getElementsCount();
		for (size_t i = 0; i < meshesCount; ++i)
		{
			mitevox::Mesh* mesh = playground->meshes.getElement(i);

			size_t meshJSONIndex = meshesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* meshJSON = meshesArrayJSONResult->getArrayItem(meshJSONIndex);
			MeshCodec::toGLTF(meshJSON, mesh, &playground->accessors, &playground->materials);
		}
	}

	void PlaygroundCodecGLTF::saveNodes(mitevox::Playground* playground, JSON* nodesArrayJSONResult)
	{
		size_t nodesCount = playground->nodes.getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mitevox::Node* node = playground->nodes.getElement(i);

			size_t nodeJSONIndex = nodesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* nodeJSON = nodesArrayJSONResult->getArrayItem(nodeJSONIndex);
			NodeCodec::toGLTF(
				nodeJSON,
				node,
				&playground->cameras,
				&playground->meshes,
				&playground->nodes,
				&playground->skeletons);
		}
	}

	void PlaygroundCodecGLTF::saveScenes(mitevox::Playground* playground, JSON* scenesArrayJSONResult)
	{
		size_t scenesCount = playground->scenes.getElementsCount();
		for (size_t i = 0; i < scenesCount; ++i)
		{
			mitevox::Scene* scene = playground->scenes.getElement(i);

			size_t sceneJSONIndex = scenesArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* sceneJSON = scenesArrayJSONResult->getArrayItem(sceneJSONIndex);
			SceneCodec::toGLTF(sceneJSON, scene, &playground->nodes);
		}
	}

	void PlaygroundCodecGLTF::saveAnimations(mitevox::Playground* playground, JSON* animationsArrayJSONResult)
	{
		/*size_t animationsCount = animationsArrayJSON->getArraySize();
		playgroundResult->animations.resize(animationsCount);

		for (size_t i = 0; i < animationsCount; ++i)
		{
			JSON* animationJSON = animationsArrayJSON->getArrayItem(i);
			mitevox::Animation* animation = new mitevox::Animation();
			AnimationCodec::fromGLTF(
				animation, animationJSON,
				&playgroundResult->nodes,
				&playgroundResult->accessors);
			playgroundResult->animations.setElement((int64_t)i, animation);
		}*/
	}

	void PlaygroundCodecGLTF::saveSkins(mitevox::Playground* playground, JSON* skinsArrayJSONResult)
	{
		size_t skinsCount = playground->skeletons.getElementsCount();
		for (size_t i = 0; i < skinsCount; ++i)
		{
			mitevox::NodeBasedSkeleton* skeleton = (mitevox::NodeBasedSkeleton*)playground->skeletons.getElement(i);

			size_t skinJSONIndex = skinsArrayJSONResult->addArrayItem(JSONtype::OBJECT);
			JSON* skinJSON = skinsArrayJSONResult->getArrayItem(skinJSONIndex);
			NodeBasedSkeletonCodec::toGLTF(
				skinJSON,
				skeleton, 
				&playground->nodes,
				&playground->accessors);
		}
	}
}