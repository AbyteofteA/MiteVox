#ifndef GLTFMESHPRIMITIVE_H
#define GLTFMESHPRIMITIVE_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Material.h"
#include "engine/CodeSafety/src/SafeArray.h"

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

    class MeshPrimitive
    {
    public:

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

        MeshAttributeSet attributes;
        BufferViewAccessor* indecesAccessor = nullptr;
        fileio::Material* material = nullptr;
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
    };
}

#endif

