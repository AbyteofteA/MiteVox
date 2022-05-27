#ifndef NODE_H
#define NODE_H

#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
//#include "engine/Math/src/LinearAlgebra/Transform.h"

#include <string>
#include <cstdint>

namespace mitevox
{
    class Node
    {
    public:

        render::Camera* camera = nullptr;
        int32_t skinIndex = -1;
        Mesh* mesh = nullptr;
        std::string name;
        //mathem::Transform transform;
        safety::SafeFloatArray weights;
        safety::SafeFloatArray matrix;
        safety::SafeArray<Node*> children;
    };
}

#endif