#ifndef GLTFBUFFERVIEW_H
#define GLTFBUFFERVIEW_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

#include <cstdint>
#include <string>

namespace mitevox
{
    class BufferView
    {
    public:

        enum TargetBufferType
        {
            ARRAY_BUFFER = 34962,
            ELEMENT_ARRAY_BUFFER = 34963
        };

        safety::SafeByteArray* buffer = nullptr;
        uint64_t byteOffset = 0;
        uint64_t byteLength = 0;
        uint16_t byteStride = 0;
        TargetBufferType target = TargetBufferType::ARRAY_BUFFER;
        std::string name;
        uint32_t ID = 0;

        void* getDataStart();
    };
}

#endif
