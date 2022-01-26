#ifndef GLTFNODE_H
#define GLTFNODE_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"
//#include "engine/Math/src/LinearAlgebra/Transform.h"

#include <string>
#include <cstdint>

namespace fileio
{
    class Node
    {
    public:

        int32_t cameraIndex;
        int32_t skinIndex;
        int32_t meshIndex;
        std::string name;
        //mathem::Transform transform;
        safety::SafeFloatArray weights;
        safety::SafeFloatArray matrix;
        safety::SafeArray<int32_t> children;

        Node();
        void fromGLTF(JSON* nodeJSON);
    };
}

#endif