#ifndef MITEVOX_NODE_H
#define MITEVOX_NODE_H

#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

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
        Mesh* meshAnimationTarget = nullptr;
        std::string name;
        safety::SafeFloatArray weights;
        mathem::GeometryTransform transform;
        safety::SafeArray<Node*> children;

        bool isAnimatedMesh();
        Mesh* getMeshToRender();
    };
}

#endif