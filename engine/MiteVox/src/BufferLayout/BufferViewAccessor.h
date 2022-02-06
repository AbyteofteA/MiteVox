#ifndef GLTFACCESSOR_H
#define GLTFACCESSOR_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessorSparse.h"
#include "engine/MiteVox/src/BufferLayout/ComponentDataType.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>
#include <string>

namespace mitevox
{
    struct BufferViewAccessor
    {
        enum struct Type
        {
            INVALID = -1,
            SCALAR,
            VEC2,
            VEC3,
            VEC4,
            MAT2,
            MAT3,
            MAT4
        };

        BufferView* bufferView = nullptr;
        uint64_t byteOffset = 0;
        ComponentDataType componentType = ComponentDataType::FLOAT;
        uint64_t count = 0;
        bool normalized = false;
        Type type = Type::INVALID;
        std::string name;

        safety::SafeFloatArray* min = nullptr;
        safety::SafeFloatArray* max = nullptr;
        BufferViewAccessorSparse* sparse = nullptr;

        BufferViewAccessor();
        void deallocate();
        bool isSparse();
        void makeSparse(BufferViewAccessorSparse* sparseAccessor);
        void makeNotSparse();
        static Type mapTypeNameToType(std::string typeName);
    };
}

#endif