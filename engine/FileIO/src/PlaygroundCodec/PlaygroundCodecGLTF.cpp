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

#include <string>

namespace fileio
{
	void PlaygroundCodecGLTF::fromGLTF(mitevox::Playground* playgroundResult, JSON* playgroundJSON, std::string filePath)
	{
		JSON* numberJSON = playgroundJSON->getField("scene");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				playgroundResult->activeScene = (int32_t)numberJSON->getNumber();
			}
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
			camera->_name = cameraJSON->getFieldString("name");
			std::string cameraType = cameraJSON->getFieldString("type");
			if (cameraType == "perspective")
			{
				camera->_type = render::CameraType::PERPECTIVE;
				JSON* perspectiveCameraJSON = cameraJSON->getFieldObject("perspective");

				double aspectRatio = cameraJSON->getFieldNumber("aspectRatio");
				camera->FOV = (float)cameraJSON->getFieldNumber("yfov");
				camera->farCullPlane = (float)cameraJSON->getFieldNumber("zfar");
				camera->nearCullPlane = (float)cameraJSON->getFieldNumber("znear");
			}
			else if (cameraType == "orthographic")
			{
				camera->_type = render::CameraType::ORTHOGRAPHIC;
				JSON* orthographicCameraJSON = cameraJSON->getFieldObject("orthographic");

				double xMagnification = cameraJSON->getFieldNumber("xmag");
				double yMagnification = cameraJSON->getFieldNumber("ymag");
				camera->farCullPlane = (float)cameraJSON->getFieldNumber("zfar");
				camera->nearCullPlane = (float)cameraJSON->getFieldNumber("znear");
			}

			playgroundResult->cameras.setElement(i, camera);
		}
	}

	void PlaygroundCodecGLTF::collectBuffers(
		mitevox::Playground* playgroundResult, 
		fileio::JSON* buffersArrayJSON, 
		std::string filePath)
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

			int64_t byteLength = (int64_t)bufferJSON->getFieldNumber("byteLength");
			safety::SafeByteArray* buffer = new safety::SafeByteArray();

			JSON* bufferURIJSON = bufferJSON->getField("uri");
			if (bufferURIJSON != nullptr)
			{
				std::string uri = bufferURIJSON->getString();
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

	void PlaygroundCodecGLTF::collectImages(
		mitevox::Playground* playgroundResult, 
		fileio::JSON* imagesArrayJSON, 
		std::string filePath)
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

			JSON* numberJSON = textureJSON->getField("sampler");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					int32_t samplerIndex = (int32_t)numberJSON->getNumber();
					texture->sampler = playgroundResult->imageSamplers.getElement(samplerIndex);
				}
			}

			numberJSON = textureJSON->getField("source");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					int32_t imageIndex = (int32_t)numberJSON->getNumber();
					texture->image = playgroundResult->images.getElement(imageIndex);
				}
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
		}
	}
}