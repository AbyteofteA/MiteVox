#ifndef NODE_CODEC_H
#define NODE_CODEC_H

#include "engine/MiteVox/src/Node.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"

namespace fileio
{
    class NodeCodec
    {
    public:

        static void fromGLTF(
            mitevox::Node* nodeResult, 
            JSON* nodeJSON,
            safety::SafeArray<render::Camera*>* cameras,
            safety::SafeArray<mitevox::Mesh*>* meshes,
            safety::SafeArray<mitevox::Node*>* nodes);
    };
}

#endif