#ifndef MITEVOX_NODE_H
#define MITEVOX_NODE_H

#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Renderer/src/RendererAPI/Light.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"

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
        mathem::ComplexGeometry* collider = nullptr;
        render::LightType lightType = render::LightType::NONE;
        render::AnyLight light;
        safety::SafeArray<Node*> children;

        bool isMorphableMesh();
        Mesh* getMeshToRender();

        void tryGenerateHitbox();

        // Methods that octree requires in order to work

        mathem::GeometryTransform* getTransform();
        mathem::ComplexGeometry* getCollider();
    };
}

#endif