
#ifndef PLAYGROUNDCODECGLTF_H
#define PLAYGROUNDCODECGLTF_H

#include "engine/MiteVox/src/Playground/Playground.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/SerializationStatus.h"

namespace fileio
{
	class PlaygroundCodecGLTF
	{
	public:

		static DeserializationStatus fromGLTF(mitevox::Playground* playgroundResult, JSON* playgroundJSON, std::string filePath); // TODO: return correct status
		static SerializationStatus toGLTF(mitevox::Playground* playground, JSON* playgroundJSONResult, std::string filePath); // TODO: return correct status

	private:

        static void collectExtensions(mitevox::Playground* playground, JSON* glTFJSON);
        static bool extensionsAreSupported(mitevox::Playground* playground);

        static void collectCameras(mitevox::Playground* playgroundResult, JSON* camerasArrayJSON);
        static void collectBuffers(mitevox::Playground* playgroundResult, JSON* buffersArrayJSON, std::string filePath);
        static void collectBufferViews(mitevox::Playground* playgroundResult, JSON* bufferViewsArrayJSON);
        static void collectAccessors(mitevox::Playground* playgroundResult, JSON* accessorsArrayJSON);
        static void collectImageSamplers(mitevox::Playground* playgroundResult, JSON* imageSamplersArrayJSON);
        static void collectImages(mitevox::Playground* playgroundResult, JSON* imagesArrayJSON, std::string filePath);
        static void collectTextures(mitevox::Playground* playgroundResult, JSON* texturesArrayJSON);
        static void collectMaterials(mitevox::Playground* playgroundResult, JSON* materialsArrayJSON);
        static void collectMeshes(mitevox::Playground* playgroundResult, JSON* meshesArrayJSON);
        static void collectNodes(mitevox::Playground* playgroundResult, JSON* nodesArrayJSON);
        static void collectScenes(mitevox::Playground* playgroundResult, JSON* scenesArrayJSON);
        static void collectAnimations(mitevox::Playground* playgroundResult, JSON* animationsArrayJSON);
        static void collectSkins(mitevox::Playground* playgroundResult, JSON* skinsArrayJSON);

        static void resolveSkeletonsPointers(mitevox::Playground* playgroundResult);
        static void preparePlayground(mitevox::Playground* playgroundResult);

        static void saveExtensions(JSON* glTFJSON, mitevox::Playground* playground);

        static void saveCameras(mitevox::Playground* playground, JSON* camerasArrayJSONResult);
        static void saveBuffers(mitevox::Playground* playground, JSON* buffersArrayJSONResult, std::string filePath);
        static void saveBufferViews(mitevox::Playground* playground, JSON* bufferViewsArrayJSONResult);
        static void saveAccessors(mitevox::Playground* playground, JSON* accessorsArrayJSONResult);
        static void saveImageSamplers(mitevox::Playground* playground, JSON* imageSamplersArrayJSONResult);
        static void saveImages(mitevox::Playground* playground, JSON* imagesArrayJSONResult, std::string filePath);
        static void saveTextures(mitevox::Playground* playground, JSON* texturesArrayJSONResult);
        static void saveMaterials(mitevox::Playground* playground, JSON* materialsArrayJSONResult);
        static void saveMeshes(mitevox::Playground* playground, JSON* meshesArrayJSONResult);
        static void saveNodes(mitevox::Playground* playground, JSON* nodesArrayJSONResult);
        static void saveScenes(mitevox::Playground* playground, JSON* scenesArrayJSONResult);
        static void saveAnimations(mitevox::Playground* playground, JSON* animationsArrayJSONResult);
        static void saveSkins(mitevox::Playground* playground, JSON* skinsArrayJSONResult);
	};
}

#endif

