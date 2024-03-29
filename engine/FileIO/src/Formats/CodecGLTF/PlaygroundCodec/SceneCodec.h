#ifndef SCENECODEC_H
#define SCENECODEC_H

#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>
#include <cstdint>

namespace fileio
{
    class SceneCodec
    {
    public:

        static void fromGLTF(mitevox::Scene* sceneResult, JSON* sceneJSON, safety::SafeArray<mitevox::Node*>* nodes);
        static void toGLTF(JSON* sceneJSONResult, mitevox::Scene* scene, safety::SafeArray<mitevox::Node*>* nodes);
    };
}

#endif

