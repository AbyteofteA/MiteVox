#ifndef GLTF_H
#define GLTF_H

#include "engine/FileIO/src/Formats/glTF_2.0/BufferLayout/BufferView.h"
#include "engine/FileIO/src/Formats/glTF_2.0/BufferLayout/BufferViewAccessor.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Mesh/Mesh.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Node.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Scene.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/ImageSampler.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Image.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Texture.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Material.h"
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

        safety::SafeArray<render::Camera> _cameras;
        safety::SafeArray<safety::SafeByteArray*> _buffers;
        safety::SafeArray<BufferView*> _bufferViews;
        safety::SafeArray<BufferViewAccessor*> _accessors;
        safety::SafeArray<Mesh*> _meshes;
        safety::SafeArray<Node*> _nodes;
        safety::SafeArray<Scene*> _scenes;
        safety::SafeArray<ImageSampler> _imageSamplers;
        safety::SafeArray<Image*> _images;
        safety::SafeArray<Texture*> _textures;
        safety::SafeArray<Material*> _materials;

        glTF();

        SerializationStatus serialize(safety::SafeByteArray* fileData);
        DeserializationStatus deserialize(safety::SafeByteArray* fileData);

        void collectExtensions(fileio::JSON* glTFJSON);
        bool extensionsAreSupported();

        /// <summary>
        /// TODO: camera.perspective.aspectRatio
        /// TODO: camera.orthographic.xmag
        /// TODO: camera.orthographic.ymag
        /// TODO: handle unrequired fields
        /// TODO: Camera::fromGLTF
        /// </summary>
        /// <param name="camerasArrayJSON"></param>
        void collectCameras(fileio::JSON* camerasArrayJSON);

        /// <summary>
        /// TODO: buffer.base64
        /// </summary>
        /// <param name="buffersArrayJSON"></param>
        void collectBuffers(fileio::JSON* buffersArrayJSON);
        void collectBufferViews(fileio::JSON* bufferViewsArrayJSON);
        void collectAccessors(fileio::JSON* accessorsArrayJSON);
        void collectImageSamplers(fileio::JSON* imageSamplersArrayJSON);

        /// <summary>
        /// TODO: construct from BufferView
        /// </summary>
        /// <param name="imagesArrayJSON"></param>
        void collectImages(fileio::JSON* imagesArrayJSON);

        /// <summary>
        /// TODO: set error texture.
        /// </summary>
        /// <param name="texturesArrayJSON"></param>
        void collectTextures(fileio::JSON* texturesArrayJSON);

        /// <summary>
        /// NOTE: it doesn't support material.normalTextureInfo (glTF)
        /// NOTE: it doesn't support material.occlusionTextureInfo (glTF)
        /// </summary>
        /// <param name="materialsArrayJSON"></param>
        void collectMaterials(fileio::JSON* materialsArrayJSON);

        void collectMeshes(fileio::JSON* meshesArrayJSON);

        /// <summary>
        /// TODO: transform
        /// </summary>
        /// <param name="nodeJSON"></param>
        void collectNodes(fileio::JSON* nodesArrayJSON);
        void collectScenes(fileio::JSON* scenesArrayJSON);
    };
}

#endif
