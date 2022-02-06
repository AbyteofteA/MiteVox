#ifndef GLTFSPARSEACCESSOR_H
#define GLTFSPARSEACCESSOR_H

#include "engine/MiteVox/src/BufferLayout/ComponentDataType.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>

namespace mitevox
{
    class BufferViewAccessorSparse
    {
    public:

        uint64_t count = 0;

        struct Indices
        {
            int32_t bufferViewIndex = -1;
            uint64_t byteOffset = 0;
            ComponentDataType componentType = ComponentDataType::FLOAT;
        } indices;

        struct Values
        {
            int32_t bufferViewIndex = -1;
            uint64_t byteOffset = 0;
        } values;
    };
}

#endif