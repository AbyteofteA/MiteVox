#ifndef GLTFMESH_H
#define GLTFMESH_H

#include "engine/Math/src/Geometry/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/TriangleGeometry.h"
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
        uint32_t getPointsCount();
        mathem::Vector3D getPoint(uint32_t index);
        mathem::Vector3D getPoint(uint32_t index, mathem::GeometryTransform* globalTransform);
        uint32_t getTrianglesCount();
        mathem::TriangleGeometry getTriangle(uint32_t index);
        mathem::TriangleGeometry getTriangle(uint32_t index, mathem::GeometryTransform* globalTransform);
        bool isIdealGeometry();
    };
}

#endif