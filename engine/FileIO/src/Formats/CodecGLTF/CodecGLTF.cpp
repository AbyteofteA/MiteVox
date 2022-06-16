#include "CodecGLTF.h"

#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/FileIO/src/BufferLayoutCodec/BufferViewCodec.h"
#include "engine/FileIO/src/BufferLayoutCodec/BufferViewAccessorCodec.h"
#include "engine/FileIO/src/MaterialCodec/ImageCodec.h"
#include "engine/FileIO/src/MaterialCodec/ImageSamplerCodec.h"
#include "engine/FileIO/src/MaterialCodec/MaterialCodec.h"
#include "engine/FileIO/src/MeshCodec/MeshCodec.h"
#include "engine/FileIO/src/PlaygroundCodec/NodeCodec.h"
#include "engine/FileIO/src/PlaygroundCodec/SceneCodec.h"
#include "engine/FileIO/src/PlaygroundCodec/PlaygroundCodecGLTF.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	CodecGLTF::CodecGLTF()
	{

	}

	SerializationStatus CodecGLTF::serialize(safety::SafeByteArray* fileData)
	{
		return SerializationStatus::ERROR_UNKNOWN;
	}

	DeserializationStatus CodecGLTF::deserialize(safety::SafeByteArray* fileData)
	{
		JSON* fileDataJSON = new JSON();
		fileDataJSON->deserialize(fileData);

		JSON* assetJSON = fileDataJSON->getFieldObject("asset");
		_asset.version = assetJSON->getFieldString("version");
		_asset.minVersion = assetJSON->getFieldString("minVersion");
		_asset.generator = assetJSON->getFieldString("generator");
		_asset.copyright = assetJSON->getFieldString("copyright");

		collectExtensions(fileDataJSON);
		if (extensionsAreSupported() == false)
		{
			return DeserializationStatus::ERROR_UNKNOWN;
		}

		result = new mitevox::Playground();
		PlaygroundCodecGLTF::fromGLTF(result, fileDataJSON, _path);

		return DeserializationStatus::COMPLETE;
	}

	void CodecGLTF::collectExtensions(fileio::JSON* glTFJSON)
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

	bool CodecGLTF::extensionsAreSupported()
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
}