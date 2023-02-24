
#ifndef PLAYGROUNDCODECGLTF_H
#define PLAYGROUNDCODECGLTF_H

#include "engine/MiteVox/src/Playground/Playground.h"

namespace fileio
{
	class PlaygroundCodecGLTF
	{
	public:

		static void fromGLTF(mitevox::Playground* playgroundResult, JSON* playgroundJSON, std::string filePath);

	private:

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
	};
}

#endif

