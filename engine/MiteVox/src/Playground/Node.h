#ifndef MITEVOX_NODE_H
#define MITEVOX_NODE_H

#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Renderer/src/RendererAPI/Light.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

#include <string>
#include <cstdint>

namespace mitevox
{
    class NodeBasedSkeleton;

    class Node
    {
    public:

        render::Camera* camera = nullptr;
        SkeletonBase* skeleton = nullptr;
        Mesh* mesh = nullptr;
        Mesh* morphAnimationTarget = nullptr;
        std::string name;
        safety::SafeFloatArray weights;
        mathem::GeometryTransform transform;
        safety::SafeArray<Node*> children;

        bool isMorphableMesh();
        Mesh* getMeshToRender();
    };
}

#endif