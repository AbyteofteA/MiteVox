#ifndef GLTFSPARSEACCESSOR_H
#define GLTFSPARSEACCESSOR_H

#include "engine/FileIO/src/Formats/glTF_2.0/BufferLayout/ComponentDataType.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>

namespace fileio
{
    class BufferViewAccessorSparse
    {
    public:

        uint64_t count;

        struct Indices
        {
            int32_t bufferViewIndex;
            uint64_t byteOffset;
            ComponentDataType componentType;
        } indices;

        struct Values
        {
            int32_t bufferViewIndex;
            uint64_t byteOffset;
        } values;

        BufferViewAccessorSparse();
        void fromGLTF(JSON* accessorSparseJSON);
    };
}

#endif