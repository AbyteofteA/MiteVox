#include "PlaygroundCodecGLTF.h"

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/BufferLayoutCodec/BufferViewCodec.h"
#include "engine/FileIO/src/BufferLayoutCodec/BufferViewAccessorCodec.h"
#include "engine/FileIO/src/MaterialCodec/ImageCodec.h"
#include "engine/FileIO/src/MaterialCodec/ImageSamplerCodec.h"
#include "engine/FileIO/src/MaterialCodec/MaterialCodec.h"
#include "engine/FileIO/src/MeshCodec/MeshCodec.h"
#include "engine/FileIO/src/PlaygroundCodec/NodeCodec.h"
#include "engine/FileIO/src/PlaygroundCodec/SceneCodec.h"
#include "engine/FileIO/src/AnimationCodec/AnimationCodec.h"
#include "engine/FileIO/src/SkeletonCodec/NodeBasedSkeletonCodec.h"

#include <string>

namespace fileio
{
	void PlaygroundCodecGLTF::fromGLTF(mitevox::Playground* playgroundResult, JSON* playgroundJSON, std::string filePath)
	{
		if (JSON* numberJSON = playgroundJSON->getField("scene"))
		{
			playgroundResult->activeScene = (int32_t)numberJSON->getNumberOrDefault(-1.0);
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
		JSON* scenesArrayJSON = playgroundJSON->getFieldArray("scenes");
		collectScenes(playgroundResult, scenesArrayJSON);
		JSON* animationsArrayJSON = playgroundJSON->getFieldArray("animations");
		collectAnimations(playgroundResult, animationsArrayJSON);
		JSON* skinsArrayJSON = playgroundJSON->getFieldArray("skins");
		collectSkins(playgroundResult, skinsArrayJSON);

		preparePlayground(playgroundResult);
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

				double aspectRatio = perspectiveCameraJSON->getFieldNumberOrDefault("aspectRatio", 1.0);
				camera->FOV = (float)perspectiveCameraJSON->getFieldNumberOrDefault("yfov", 1.0);
				camera->farCullPlane = (float)perspectiveCameraJSON->getFieldNumberOrDefault("zfar", 100.0);
				camera->nearCullPlane = (float)perspectiveCameraJSON->getFieldNumberOrDefault("znear", 0.001);
			}
			else if (cameraType == "orthographic")
			{
				camera->_type = render::CameraType::ORTHOGRAPHIC;
				JSON* orthographicCameraJSON = cameraJSON->getFieldObject("orthographic");

				double xMagnification = orthographicCameraJSON->getFieldNumberOrDefault("xmag", 1.0);
				double yMagnification = orthographicCameraJSON->getFieldNumberOrDefault("ymag", 1.0);
				camera->farCullPlane = (float)orthographicCameraJSON->getFieldNumberOrDefault("zfar", 100.0);
				camera->nearCullPlane = (float)orthographicCameraJSON->getFieldNumberOrDefault("znear", 0.001);
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
		playgroundResult->buffers.fillWithZeros();

		for (int64_t i = 0; i < buffersCount; ++i)
		{
			JSON* bufferJSON = buffersArrayJSON->getArrayItem(i);

			size_t byteLength = (size_t)bufferJSON->getFieldNumberOrDefault("byteLength", 0.0);
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
		playgroundResult->bufferViews.fillWithZeros();

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
		playgroundResult->accessors.fillWithZeros();

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
			mitevox::ImageSampler* imageSampler = new mitevox::ImageSampler();
			ImageSamplerCodec::fromGLTF(imageSampler, imageSamplerJSON);
			playgroundResult->imageSamplers.setElement(i, *imageSampler);
			delete imageSampler;
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
		playgroundResult->images.fillWithZeros();

		for (int64_t i = 0; i < imagesCount; ++i)
		{
			JSON* imageJSON = imagesArrayJSON->getArrayItem(i);
			mitevox::Image* image = new mitevox::Image();
			ImageCodec::fromGLTF(image, imageJSON, filePath);
			playgroundResult->images.setElement(i, image);
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
		playgroundResult->textures.fillWithZeros();

		for (int64_t i = 0; i < texturesCount; ++i)
		{
			JSON* textureJSON = texturesArrayJSON->getArrayItem(i);

			mitevox::Texture* texture = new mitevox::Texture();

			if (JSON* numberJSON = textureJSON->getField("sampler"))
			{
				int32_t samplerIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				texture->sampler = playgroundResult->imageSamplers.getElement(samplerIndex);
			}

			if (JSON* numberJSON = textureJSON->getField("source"))
			{
				int32_t imageIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				texture->image = playgroundResult->images.getElement(imageIndex);
			}

			playgroundResult->textures.setElement(i, texture);
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
		playgroundResult->materials.fillWithZeros();

		for (int64_t i = 0; i < materialsCount; ++i)
		{
			JSON* materialJSON = materialsArrayJSON->getArrayItem(i);
			mitevox::Material* material = new mitevox::Material();
			MaterialCodec::fromGLTF(material, materialJSON, &playgroundResult->textures);
			playgroundResult->materials.setElement(i, material);
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
		playgroundResult->meshes.fillWithZeros();

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
		playgroundResult->nodes.fillWithZeros();

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
		playgroundResult->scenes.fillWithZeros();

		for (size_t i = 0; i < scenesCount; ++i)
		{
			JSON* sceneJSON = scenesArrayJSON->getArrayItem(i);
			mitevox::Scene* scene = new mitevox::Scene();
			SceneCodec::fromGLTF(scene, sceneJSON, &playgroundResult->nodes);
			playgroundResult->scenes.setElement((int64_t)i, scene);

			if (scene->activeCameraNode == nullptr)
			{
				size_t nodesCount = scene->nodes.getElementsCount();
				for (size_t i = 0; i < nodesCount; ++i)
				{
					mitevox::Node* node = scene->nodes.getElement(i);
					if (node->camera)
					{
						scene->activeCameraNode = node;
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
		playgroundResult->animations.fillWithZeros();

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
		playgroundResult->skeletons.fillWithZeros();

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

	void PlaygroundCodecGLTF::prepareNodeRecursively(mitevox::Playground* playgroundResult, mitevox::Node* node)
	{
		if (node->mesh)
		{
			if (node->isMorphableMesh())
			{
				node->morphAnimationTarget = new mitevox::Mesh();
				node->mesh->makeCopyForAnimationTo(node->morphAnimationTarget);
			}

			// Apply default material where needed
			size_t meshPrimitivesCount = node->mesh->primitives.getElementsCount();
			for (size_t i = 0; i < meshPrimitivesCount; ++i)
			{
				mitevox::MeshPrimitive* meshPrimitive = node->mesh->primitives.getElement(i);
				if (meshPrimitive->material == nullptr)
				{
					meshPrimitive->material = new mitevox::Material();
				}
			}
		}
		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			prepareNodeRecursively(playgroundResult, node->children.getElement(i));
		}
	}

	void PlaygroundCodecGLTF::preparePlayground(mitevox::Playground* playgroundResult)
	{
		// Resolve skeletons pointers from indeces
		size_t nodesCount = playgroundResult->nodes.getElementsCount();
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mitevox::Node* node = playgroundResult->nodes.getElement(i);
			if (node->skeleton != 0)
			{
				node->skeleton = playgroundResult->skeletons.getElement((size_t)node->skeleton - 1);
			}
		}

		// Prepare nodes
		for (size_t i = 0; i < nodesCount; ++i)
		{
			mitevox::Node* node = playgroundResult->nodes.getElement(i);
			prepareNodeRecursively(playgroundResult, node);
		}

		// Generate tangents if needed
		size_t meshesCount = playgroundResult->meshes.getElementsCount();
		for (size_t i = 0; i < meshesCount; ++i)
		{
			mitevox::Mesh* mesh = playgroundResult->meshes.getElement(i);
			mesh->tryGenerateTangents();
		}
	}
}