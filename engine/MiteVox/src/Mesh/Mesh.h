#ifndef GLTFMESH_H
#define GLTFMESH_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace mitevox
{
    class Mesh :
        public mathem::GeometryPrimitiveBase,
        public mathem::GeometryTransform
    {
    public:

        safety::SafeArray<MeshPrimitive*> primitives;
        safety::SafeFloatArray weights;
        std::string name;

        explicit Mesh();
        bool isTriangularMesh();
        uint32_t getVertecesCount();
        mathem::Vector3D getVertexPosition(uint32_t index);
        uint32_t getTrianglesCount();
        mathem::TriangleGeometry getTrianglePositions(uint32_t index);
        bool isIdealGeometry();
    };
}

#endif