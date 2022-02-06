#include "glTF.h"

#include "engine/FileIO/src/BufferLayoutCodec/BufferViewCodec.h"
#include "engine/FileIO/src/BufferLayoutCodec/BufferViewAccessorCodec.h"
#include "engine/FileIO/src/Mesh/MeshCodec.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	glTF::glTF() :
		_sceneIndex(-1)
	{

	}

	SerializationStatus glTF::serialize(safety::SafeByteArray* fileData)
	{
		return SerializationStatus::ERROR;
	}

	DeserializationStatus glTF::deserialize(safety::SafeByteArray* fileData)
	{
		JSON* fileDataJSON = new JSON();
		fileDataJSON->deserialize(fileData);

		JSON* numberJSON = fileDataJSON->getField("scene");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				_sceneIndex = (int32_t)numberJSON->getNumber();
			}
		}

		JSON* assetJSON = fileDataJSON->getFieldObject("asset");
		_asset.version = assetJSON->getFieldString("version");
		_asset.minVersion = assetJSON->getFieldString("minVersion");
		_asset.generator = assetJSON->getFieldString("generator");
		_asset.copyright = assetJSON->getFieldString("copyright");

		collectExtensions(fileDataJSON);
		if (extensionsAreSupported() == false)
		{
			return DeserializationStatus::ERROR;
		}

		JSON* camerasArrayJSON = fileDataJSON->getFieldArray("cameras");
		collectCameras(camerasArrayJSON);

		JSON* buffersArrayJSON = fileDataJSON->getFieldArray("buffers");
		collectBuffers(buffersArrayJSON);
		JSON* bufferViewsArrayJSON = fileDataJSON->getFieldArray("bufferViews");
		collectBufferViews(bufferViewsArrayJSON);
		JSON* accessorsArrayJSON = fileDataJSON->getFieldArray("accessors");
		collectAccessors(accessorsArrayJSON);

		JSON* imageSamplerJSON = fileDataJSON->getFieldArray("samplers");
		collectImageSamplers(imageSamplerJSON);
		JSON* imagesJSON = fileDataJSON->getFieldArray("images");
		collectImages(imagesJSON);
		JSON* texturesJSON = fileDataJSON->getFieldArray("textures");
		collectTextures(texturesJSON);
		JSON* materialsJSON = fileDataJSON->getFieldArray("materials");
		collectMaterials(materialsJSON);

		JSON* meshesArrayJSON = fileDataJSON->getFieldArray("meshes");
		collectMeshes(meshesArrayJSON);

		JSON* nodesArrayJSON = fileDataJSON->getFieldArray("nodes");
		collectNodes(nodesArrayJSON);
		JSON* scenesArrayJSON = fileDataJSON->getFieldArray("scenes");
		collectScenes(scenesArrayJSON);

		return DeserializationStatus::COMPLETE;
	}

	void glTF::collectExtensions(fileio::JSON* glTFJSON)
	{
		if (glTFJSON == nullptr)
		{
			return;
		}

		fileio::JSON* extensionsUsedJSON = glTFJSON->getFieldArray("extensionsUsed");
		if (extensionsUsedJSON != nullptr)
		{
			extensionsUsedJSON->toStringArray(&_extensionsUsed);
		}

		fileio::JSON* extensionsRequiredJSON = glTFJSON->getFieldArray("extensionsRequired");
		if (extensionsRequiredJSON != nullptr)
		{
			extensionsRequiredJSON->toStringArray(&_extensionsRequired);
		}
	}

	bool glTF::extensionsAreSupported()
	{
		int64_t extensionsUsedCount = _extensionsUsed.getElementsCount();
		int64_t extensionsRequiredCount = _extensionsRequired.getElementsCount();
		bool extensionIsSupported = false;
		for (int64_t indexRequired = 0; indexRequired < extensionsRequiredCount; ++indexRequired)
		{
			extensionIsSupported = false;
			std::string extensionRequired = _extensionsRequired.getElement(indexRequired);
			for (int64_t indexUsed = 0; indexUsed < extensionsUsedCount; ++indexUsed)
			{
				std::string extensionUsed = _extensionsUsed.getElement(indexUsed);
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

	void glTF::collectCameras(fileio::JSON* camerasArrayJSON)
	{
		if (camerasArrayJSON == nullptr)
		{
			return;
		}

		int64_t camerasCount = camerasArrayJSON->getArraySize();
		_cameras.resize(camerasCount);

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

			_cameras.setElement(i, camera);
		}
	}

	void glTF::collectBuffers(fileio::JSON* buffersArrayJSON)
	{
		if (buffersArrayJSON == nullptr)
		{
			return;
		}

		int64_t buffersCount = buffersArrayJSON->getArraySize();
		_buffers.resize(buffersCount);
		_buffers.fillWithZeros();

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
					uri = fs::path(_path).parent_path().string() + "\\" + uri;
				}
				fileio::FileStatus fileStatus =
					fileio::FileInputOutput::loadBinary(uri, buffer);
			}

			_buffers.setElement(i, buffer);
		}
	}

	void glTF::collectBufferViews(fileio::JSON* bufferViewsArrayJSON)
	{
		if (bufferViewsArrayJSON == nullptr)
		{
			return;
		}

		int64_t bufferViewsCount = bufferViewsArrayJSON->getArraySize();
		_bufferViews.resize(bufferViewsCount);
		_bufferViews.fillWithZeros();

		for (int64_t i = 0; i < bufferViewsCount; ++i)
		{
			JSON* bufferViewJSON = bufferViewsArrayJSON->getArrayItem(i);
			mitevox::BufferView* bufferView = BufferViewCodec::fromGLTF(bufferViewJSON, &_buffers);
			_bufferViews.setElement(i, bufferView);
		}
	}

	void glTF::collectAccessors(fileio::JSON* accessorsArrayJSON)
	{
		if (accessorsArrayJSON == nullptr)
		{
			return;
		}

		int64_t accessorsCount = accessorsArrayJSON->getArraySize();
		_accessors.resize(accessorsCount);
		_accessors.fillWithZeros();

		for (int64_t i = 0; i < accessorsCount; ++i)
		{
			JSON* accessorJSON = accessorsArrayJSON->getArrayItem(i);
			mitevox::BufferViewAccessor* accessor = 
				BufferViewAccessorCodec::fromGLTF(accessorJSON, &_bufferViews);
			_accessors.setElement(i, accessor);
		}
	}

	void glTF::collectImageSamplers(fileio::JSON* imageSamplersArrayJSON)
	{
		if (imageSamplersArrayJSON == nullptr)
		{
			return;
		}

		int64_t imageSamplersCount = imageSamplersArrayJSON->getArraySize();
		_imageSamplers.resize(imageSamplersCount);

		for (int64_t i = 0; i < imageSamplersCount; ++i)
		{
			JSON* imageSamplerJSON = imageSamplersArrayJSON->getArrayItem(i);
			ImageSampler imageSampler;
			imageSampler.fromGLTF(imageSamplerJSON);
			_imageSamplers.setElement(i, imageSampler);
		}
	}

	void glTF::collectImages(fileio::JSON* imagesArrayJSON)
	{
		if (imagesArrayJSON == nullptr)
		{
			return;
		}

		int64_t imagesCount = imagesArrayJSON->getArraySize();
		_images.resize(imagesCount);
		_images.fillWithZeros();

		for (int64_t i = 0; i < imagesCount; ++i)
		{
			JSON* imageJSON = imagesArrayJSON->getArrayItem(i);
			Image* image = new Image();
			image->fromGLTF(imageJSON, _path);
			_images.setElement(i, image);
		}
	}

	void glTF::collectTextures(fileio::JSON* texturesArrayJSON)
	{
		if (texturesArrayJSON == nullptr)
		{
			return;
		}

		int64_t texturesCount = texturesArrayJSON->getArraySize();
		_textures.resize(texturesCount);
		_textures.fillWithZeros();

		for (int64_t i = 0; i < texturesCount; ++i)
		{
			JSON* textureJSON = texturesArrayJSON->getArrayItem(i);

			Texture* texture = new Texture();

			JSON* numberJSON = textureJSON->getField("sampler");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					int32_t samplerIndex = (int32_t)numberJSON->getNumber();
					texture->sampler = _imageSamplers.getElement(samplerIndex);
				}
			}

			numberJSON = textureJSON->getField("source");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					int32_t imageIndex = (int32_t)numberJSON->getNumber();
					texture->image = _images.getElement(imageIndex);
				}
			}

			_textures.setElement(i, texture);
		}
	}

	void glTF::collectMaterials(fileio::JSON* materialsArrayJSON)
	{
		if (materialsArrayJSON == nullptr)
		{
			return;
		}

		int64_t materialsCount = materialsArrayJSON->getArraySize();
		_materials.resize(materialsCount);
		_materials.fillWithZeros();

		for (int64_t i = 0; i < materialsCount; ++i)
		{
			JSON* materialJSON = materialsArrayJSON->getArrayItem(i);
			Material* material = new Material();
			material->fromGLTF(materialJSON, &_textures);
			_materials.setElement(i, material);
		}
	}

	void glTF::collectMeshes(fileio::JSON* meshesArrayJSON)
	{
		if (meshesArrayJSON == nullptr)
		{
			return;
		}

		size_t meshesCount = meshesArrayJSON->getArraySize();
		_meshes.resize(meshesCount);
		_meshes.fillWithZeros();

		for (size_t i = 0; i < meshesCount; ++i)
		{
			JSON* meshJSON = meshesArrayJSON->getArrayItem(i);
			mitevox::Mesh* mesh = MeshCodec::fromGLTF(meshJSON, &_accessors, &_materials);
			_meshes.setElement((int64_t)i, mesh);
		}
	}

	void glTF::collectNodes(fileio::JSON* nodesArrayJSON)
	{
		if (nodesArrayJSON == nullptr)
		{
			return;
		}

		size_t nodesCount = nodesArrayJSON->getArraySize();
		_nodes.resize(nodesCount);
		_nodes.fillWithZeros();

		for (size_t i = 0; i < nodesCount; ++i)
		{
			_nodes.setElement((int64_t)i, new Node());
		}

		for (size_t i = 0; i < nodesCount; ++i)
		{
			JSON* nodeJSON = nodesArrayJSON->getArrayItem(i);
			Node* node = _nodes.getElement(i);
			node->fromGLTF(nodeJSON, &_cameras, &_meshes, &_nodes);
		}
	}

	void glTF::collectScenes(fileio::JSON* scenesArrayJSON)
	{
		if (scenesArrayJSON == nullptr)
		{
			return;
		}

		size_t scenesCount = scenesArrayJSON->getArraySize();
		_scenes.resize(scenesCount);
		_scenes.fillWithZeros();

		for (size_t i = 0; i < scenesCount; ++i)
		{
			JSON* sceneJSON = scenesArrayJSON->getArrayItem(i);
			Scene* scene = new Scene();
			scene->fromGLTF(sceneJSON, &_nodes);
			_scenes.setElement((int64_t)i, scene);
		}
	}
}