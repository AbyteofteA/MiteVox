#ifndef GLTFMESH_H
#define GLTFMESH_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace mitevox
{
    class Mesh :
        public mathem::GeometryPrimitiveBase
    {
    public:

        safety::SafeArray<MeshPrimitive*> primitives;
        safety::SafeFloatArray weights;
        std::string name;

        bool isUploaded = false;

        explicit Mesh();

        MeshPrimitive* addMeshPrimitive();

        void makeCopyForAnimationTo(Mesh* resultMesh);
        void tryGenerateTangents();

        mathem::Vector3D getMinPosition();
        mathem::Vector3D getMaxPosition();

        bool isTriangularMesh();
        size_t getVertecesCount();
        mathem::Vector3D getVertexPosition(uint32_t index);
        size_t getTrianglesCount();
        mathem::TriangleGeometry3D getTrianglePositions(uint32_t index);
        bool isIdealGeometry();

        void setVertexPosition(uint32_t index, mathem::Vector3D value);

        bool isMorphable();
        size_t getMorphTargetsCount();
    };
}

#endif