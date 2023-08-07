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

        std::string name;
        mathem::GeometryTransform transform;
        render::Camera* camera = nullptr;
        SkeletonBase* skeleton = nullptr;
        Mesh* mesh = nullptr;
        Mesh* morphAnimationTarget = nullptr;
        safety::SafeFloatArray weights;

        render::LightType lightType = render::LightType::NONE;
        render::AnyLight light;
        safety::SafeArray<Node*> children;

        bool isMorphableMesh();
        Mesh* getMeshToRender();

        Mesh* tryAttachNewMesh();

        bool hasCameraRecursively();
        mathem::GeometryTransform getCameraRecursively(render::Camera** cameraResult, mathem::GeometryTransform* parentTransform);

        void getMinMaxRecursively(mathem::GeometryTransform* parentTransform, mathem::Vector3D* min, mathem::Vector3D* max);
    };
}

#endif