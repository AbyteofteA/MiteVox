#ifndef GLTFMESHPRIMITIVE_H
#define GLTFMESHPRIMITIVE_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Geometry/TriangleGeometry.h"

#include <string>
#include <cstdint>

namespace mitevox
{
    class MeshAttributeSet
    {
    public:

        BufferViewAccessor* positionAccessor = nullptr;
        BufferViewAccessor* normalAccessor = nullptr;
        BufferViewAccessor* tangentAccessor = nullptr;
        BufferViewAccessor* textureCoordAccessor_0 = nullptr;
        BufferViewAccessor* textureCoordAccessor_1 = nullptr;
        BufferViewAccessor* colorAccessor_0 = nullptr;
        BufferViewAccessor* jointsAccessor_0 = nullptr;
        BufferViewAccessor* weightsAccessor_0 = nullptr;
    };

    enum TopologyType
    {
        POINTS = 0,
        LINES = 1,
        LINE_LOOP = 2,
        LINE_STRIP = 3,
        TRIANGLES = 4,
        TRIANGLE_STRIP = 5,
        TRIANGLE_FAN = 6
    };

    class MeshPrimitive
    {
    public:

        MeshAttributeSet attributes;
        BufferViewAccessor* indecesAccessor = nullptr;
        Material* material = nullptr;
        TopologyType mode = TopologyType::TRIANGLES;
        safety::SafeArray<MeshAttributeSet*> morphTargets;
        uint32_t ID = 0;

        BufferViewAccessor* getPositions();
        BufferViewAccessor* getNormals();
        BufferViewAccessor* getTangents();
        BufferViewAccessor* getTextureCoords_0();
        BufferViewAccessor* getTextureCoords_1();
        BufferViewAccessor* getColors_0();
        BufferViewAccessor* getJoints_0();
        BufferViewAccessor* getWeights_0();
        BufferViewAccessor* getIndeces();

        bool isTriangularMesh();
        uint32_t getPointsCount();
        mathem::Vector3D getPoint(uint32_t index);
        uint32_t getTrianglesCount();
        mathem::TriangleGeometry getTriangle(uint32_t index);
        /// TODO: getTriangleNormals(uint32_t index);
        /// TODO: getTriangleTangents(uint32_t index);
        /// TODO: getTriangleTextureCoords_0(uint32_t index);
        /// TODO: getTriangleTextureCoords_1(uint32_t index);
        /// TODO: getTriangleColors_0(uint32_t index);
        /// TODO: getTriangleJoints_0(uint32_t index);
        /// TODO: getTriangleWeights_0(uint32_t index);
        /// TODO: getTriangleIndeces(uint32_t index);
    };
}

#endif

