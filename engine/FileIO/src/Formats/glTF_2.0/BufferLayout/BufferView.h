#ifndef GLTFBUFFERVIEW_H
#define GLTFBUFFERVIEW_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

#include <cstdint>
#include <string>

namespace fileio
{
    class BufferView
    {
    public:

        enum TargetBufferType
        {
            ARRAY_BUFFER = 34962,
            ELEMENT_ARRAY_BUFFER = 34963
        };

        safety::SafeByteArray* buffer;
        uint64_t byteOffset;
        uint64_t byteLength;
        uint16_t byteStride;
        TargetBufferType target;
        std::string name;
        uint32_t ID = 0;

        BufferView();
        void fromGLTF(JSON* bufferViewJSON, safety::SafeArray<safety::SafeByteArray*>* buffers);
    };
}

#endif
