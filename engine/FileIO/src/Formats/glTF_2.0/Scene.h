#ifndef GLTFSCENE_H
#define GLTFSCENE_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Node.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>
#include <cstdint>

namespace fileio
{
    class Scene
    {
    public:

        std::string _name;
        safety::SafeArray<Node*> _nodes;

        Scene();
        void fromGLTF(JSON* sceneJSON, safety::SafeArray<Node*>* nodes);
    };
}

#endif

