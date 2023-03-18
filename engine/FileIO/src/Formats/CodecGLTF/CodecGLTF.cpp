#include "CodecGLTF.h"

#include "engine/MiteVox/src/Playground/Playground.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/FileIO/src/Formats/CodecGLTF/PlaygroundCodec/PlaygroundCodecGLTF.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	CodecGLTF::CodecGLTF(mitevox::Playground* playgroundResult)
	{
		result = playgroundResult;
	}

	SerializationStatus CodecGLTF::serialize(safety::SafeByteArray* fileData)
	{
		JSON* fileDataJSON = new JSON();
		SerializationStatus serializationStatus = PlaygroundCodecGLTF::toGLTF(result, fileDataJSON, _path);
		fileDataJSON->serialize(fileData);
		delete fileDataJSON;

		return serializationStatus;
	}

	DeserializationStatus CodecGLTF::deserialize(safety::SafeByteArray* fileData)
	{
		JSON* fileDataJSON = new JSON();
		fileDataJSON->deserialize(fileData);
		DeserializationStatus deserializationStatus = PlaygroundCodecGLTF::fromGLTF(result, fileDataJSON, _path);
		delete fileDataJSON;

		return deserializationStatus;
	}
}