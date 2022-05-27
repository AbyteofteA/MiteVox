#ifndef GLTF_H
#define GLTF_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Node.h"
#include "engine/MiteVox/src/Scene.h"
#include "engine/MiteVox/src/Material/ImageSampler.h"
#include "engine/MiteVox/src/Material/Image.h"
#include "engine/MiteVox/src/Material/Texture.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/FileIO/src/Formats/FileFormatBase.h"
#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"

#include <string>

namespace fileio
{
    class glTF : public FileFormatBase
    {
    public:

        int32_t _sceneIndex;

        struct Asset
        {
            std::string version;
            std::string minVersion;
            std::string generator;
            std::string copyright;
        }_asset;

        safety::SafeArray<std::string> _extensionsUsed;
        safety::SafeArray<std::string> _extensionsRequired;

        safety::SafeArray<render::Camera*> _cameras;
        safety::SafeArray<safety::SafeByteArray*> _buffers;
        safety::SafeArray<mitevox::BufferView*> _bufferViews;
        safety::SafeArray<mitevox::BufferViewAccessor*> _accessors;
        safety::SafeArray<mitevox::Mesh*> _meshes;
        safety::SafeArray<mitevox::Node*> _nodes;
        safety::SafeArray<mitevox::Scene*> _scenes;
        safety::SafeArray<mitevox::ImageSampler> _imageSamplers;
        safety::SafeArray<mitevox::Image*> _images;
        safety::SafeArray<mitevox::Texture*> _textures;
        safety::SafeArray<mitevox::Material*> _materials;

        glTF();

        SerializationStatus serialize(safety::SafeByteArray* fileData);
        DeserializationStatus deserialize(safety::SafeByteArray* fileData);

    private:

        void collectExtensions(JSON* glTFJSON);
        bool extensionsAreSupported();

        /// <summary>
        /// TODO: camera.perspective.aspectRatio
        /// TODO: camera.orthographic.xmag
        /// TODO: camera.orthographic.ymag
        /// TODO: handle unrequired fields
        /// TODO: Camera::fromGLTF
        /// </summary>
        /// <param name="camerasArrayJSON"></param>
        void collectCameras(JSON* camerasArrayJSON);

        /// <summary>
        /// TODO: buffer.base64
        /// </summary>
        /// <param name="buffersArrayJSON"></param>
        void collectBuffers(JSON* buffersArrayJSON);
        void collectBufferViews(JSON* bufferViewsArrayJSON);
        void collectAccessors(JSON* accessorsArrayJSON);
        void collectImageSamplers(JSON* imageSamplersArrayJSON);

        /// <summary>
        /// TODO: construct from BufferView
        /// </summary>
        /// <param name="imagesArrayJSON"></param>
        void collectImages(JSON* imagesArrayJSON);

        /// <summary>
        /// TODO: set error texture.
        /// </summary>
        /// <param name="texturesArrayJSON"></param>
        void collectTextures(JSON* texturesArrayJSON);

        /// <summary>
        /// NOTE: it doesn't support material.normalTextureInfo (glTF)
        /// NOTE: it doesn't support material.occlusionTextureInfo (glTF)
        /// </summary>
        /// <param name="materialsArrayJSON"></param>
        void collectMaterials(JSON* materialsArrayJSON);

        void collectMeshes(JSON* meshesArrayJSON);

        /// <summary>
        /// TODO: transform
        /// </summary>
        /// <param name="nodeJSON"></param>
        void collectNodes(JSON* nodesArrayJSON);
        void collectScenes(JSON* scenesArrayJSON);
    };
}

#endif
