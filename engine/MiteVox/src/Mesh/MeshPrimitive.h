#ifndef GLTFMESHPRIMITIVE_H
#define GLTFMESHPRIMITIVE_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/Mesh/MeshAttributeSet.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry2D.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

#include <string>
#include <cstdint>

namespace mitevox
{
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
        TopologyType topologyType = TopologyType::TRIANGLES;
        safety::SafeArray<MeshAttributeSet*> morphTargets;
        uint32_t ID = 0;

        void makeCopyForAnimationTo(MeshPrimitive* resultMeshPrimitive);
        void tryGenerateTangents();

        BufferViewAccessor* getPositions();
        BufferViewAccessor* getNormals();
        BufferViewAccessor* getTangents();
        BufferViewAccessor* getTextureCoords_0();
        BufferViewAccessor* getTextureCoords_1();
        BufferViewAccessor* getColors_0();
        BufferViewAccessor* getJoints_0();
        BufferViewAccessor* getWeights_0();
        BufferViewAccessor* getIndeces();

        mathem::Vector3D getMinPosition();
        mathem::Vector3D getMaxPosition();

        bool isTriangularMesh();
        uint32_t getVertecesCount();
        mathem::Vector3D getVertexPosition(uint32_t index);
        mathem::Vector3D getVertexNormal(uint32_t index);
        /// TODO: mathem::Vector3D getVertexTangent(uint32_t index);
        mathem::Vector2D getVertexTextureCoords_0(uint32_t index);
        mathem::Vector2D getVertexTextureCoords_1(uint32_t index);
        /// TODO: getVertexColors_0(uint32_t index);
        /// TODO: getVertexJoints_0(uint32_t index);
        /// TODO: getVertexWeights_0(uint32_t index);
        /// TODO: getVertexIndeces(uint32_t index);

        uint32_t getTrianglesCount();
        void getTriangleVertexIndeces(uint32_t triangleIndex, uint32_t* vertex1, uint32_t* vertex2, uint32_t* vertex3);
        mathem::TriangleGeometry3D getTrianglePositions(uint32_t index);
        mathem::TriangleGeometry3D getTriangleNormals(uint32_t index);
        /// TODO: mathem::TriangleGeometry3D getTriangleTangents(uint32_t index);
        mathem::TriangleGeometry2D getTriangleTextureCoords_0(uint32_t index);
        mathem::TriangleGeometry2D getTriangleTextureCoords_1(uint32_t index);
        /// TODO: getTriangleColors_0(uint32_t index);
        /// TODO: getTriangleJoints_0(uint32_t index);
        /// TODO: getTriangleWeights_0(uint32_t index);
        /// TODO: getTriangleIndeces(uint32_t index);
        
        void setVertexPosition(uint32_t index, mathem::Vector3D position);
        void setVertexNormal(uint32_t index, mathem::Vector3D normal);
        /// TODO: void setVertexTangent(uint32_t index);
        /// TODO: void setVertexTextureCoords_0(uint32_t index);
        /// TODO: void setVertexTextureCoords_1(uint32_t index);
        /// TODO: void setVertexColors_0(uint32_t index);
        /// TODO: void setVertexJoints_0(uint32_t index);
        /// TODO: void setVertexWeights_0(uint32_t index);
        /// TODO: void setVertexIndeces(uint32_t index);
        
        mathem::Vector3D getMorphVertexPosition(uint32_t morphIndex, uint32_t index);
        mathem::Vector3D getMorphVertexNormal(uint32_t morphIndex, uint32_t index);
        void setMorphVertexPosition(uint32_t morphIndex, uint32_t index, mathem::Vector3D position);
        void setMorphVertexNormal(uint32_t morphIndex, uint32_t index, mathem::Vector3D normal);
    };
}

#endif

