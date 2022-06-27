
#ifndef PLAYGROUNDCODECGLTF_H
#define PLAYGROUNDCODECGLTF_H

#include "engine/MiteVox/src/Playground.h"

namespace fileio
{
	class PlaygroundCodecGLTF
	{
	public:

		static void fromGLTF(mitevox::Playground* playgroundResult, JSON* playgroundJSON, std::string filePath);

	private:

        /// <summary>
        /// TODO: camera.perspective.aspectRatio
        /// TODO: camera.orthographic.xmag
        /// TODO: camera.orthographic.ymag
        /// TODO: handle unrequired fields
        /// TODO: Camera::fromGLTF
        /// </summary>
        /// <param name="camerasArrayJSON"></param>
        static void collectCameras(mitevox::Playground* playgroundResult, JSON* camerasArrayJSON);

        /// <summary>
        /// TODO: buffer.base64
        /// </summary>
        /// <param name="buffersArrayJSON"></param>
        static void collectBuffers(
            mitevox::Playground* playgroundResult, 
            JSON* buffersArrayJSON,
            std::string filePath);
        static void collectBufferViews(mitevox::Playground* playgroundResult, JSON* bufferViewsArrayJSON);
        static void collectAccessors(mitevox::Playground* playgroundResult, JSON* accessorsArrayJSON);
        static void collectImageSamplers(mitevox::Playground* playgroundResult, JSON* imageSamplersArrayJSON);

        /// <summary>
        /// TODO: construct from BufferView
        /// </summary>
        /// <param name="imagesArrayJSON"></param>
        static void collectImages(
            mitevox::Playground* playgroundResult, 
            JSON* imagesArrayJSON,
            std::string filePath);

        /// <summary>
        /// TODO: set error texture.
        /// </summary>
        /// <param name="texturesArrayJSON"></param>
        static void collectTextures(mitevox::Playground* playgroundResult, JSON* texturesArrayJSON);

        /// <summary>
        /// NOTE: it doesn't support material.normalTextureInfo (glTF)
        /// NOTE: it doesn't support material.occlusionTextureInfo (glTF)
        /// </summary>
        /// <param name="materialsArrayJSON"></param>
        static void collectMaterials(mitevox::Playground* playgroundResult, JSON* materialsArrayJSON);

        static void collectMeshes(mitevox::Playground* playgroundResult, JSON* meshesArrayJSON);
        static void collectNodes(mitevox::Playground* playgroundResult, JSON* nodesArrayJSON);
        static void collectScenes(mitevox::Playground* playgroundResult, JSON* scenesArrayJSON);
        static void collectAnimations(mitevox::Playground* playgroundResult, JSON* animationsArrayJSON);
	};
}

#endif

