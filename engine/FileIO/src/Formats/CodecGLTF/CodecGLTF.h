#ifndef GLTF_H
#define GLTF_H

#include "engine/MiteVox/src/Playground/Playground.h"
#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace fileio
{
    /// <summary>
    /// ### CodecGLTF implementation notes:
    /// - Camera
    ///     - TODO: camera.perspective.aspectRatio
    ///     - TODO: camera.orthographic.xmag
    ///     - TODO: camera.orthographic.ymag
    ///     - TODO: handle unrequired fields
    ///     - TODO: Camera::fromGLTF
    /// - Buffer
    ///     - TODO: buffer.base64
    /// - Accessor
    ///     - TODO: accessors.sparse
    /// - Image
    ///     - TODO: construct from BufferView ???
    /// - Texture
    ///     - TODO: set error texture (in preparePlayground)
    /// - Material
    ///     - NOTE: it doesn't support generation of material.normalTextureInfo if not passed (glTF)
    /// </summary>

    class CodecGLTF : public FileFormatBase
    {
    public:

        mitevox::Playground* result = nullptr;

        CodecGLTF(mitevox::Playground* playgroundResult);

        SerializationStatus serialize(safety::SafeByteArray* fileData);
        DeserializationStatus deserialize(safety::SafeByteArray* fileData);
    };
}

#endif
