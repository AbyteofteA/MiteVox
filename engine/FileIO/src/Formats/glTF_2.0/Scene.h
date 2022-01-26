#ifndef GLTFSCENE_H
#define GLTFSCENE_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>
#include <cstdint>

namespace fileio
{
    class Scene
    {
    public:

        std::string name;
        safety::SafeArray<int32_t> nodes;

        Scene();
        void fromGLTF(JSON* sceneJSON);
    };
}

#endif

