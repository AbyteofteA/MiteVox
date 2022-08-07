#ifndef GLTF_H
#define GLTF_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/Playground/Playground.h"
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
    class CodecGLTF : public FileFormatBase
    {
    public:

        struct Asset
        {
            std::string version;
            std::string minVersion;
            std::string generator;
            std::string copyright;
        }_asset;

        safety::SafeArray<std::string> _extensionsUsed;
        safety::SafeArray<std::string> _extensionsRequired;

        mitevox::Playground* result = nullptr;

        CodecGLTF();

        SerializationStatus serialize(safety::SafeByteArray* fileData);
        DeserializationStatus deserialize(safety::SafeByteArray* fileData);

    private:

        void collectExtensions(JSON* glTFJSON);
        bool extensionsAreSupported();
    };
}

#endif
