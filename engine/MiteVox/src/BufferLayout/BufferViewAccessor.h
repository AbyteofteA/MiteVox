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

        int8_t getByte(int64_t index);
        uint8_t getUnsignedByte(int64_t index);
        int16_t getShort(int64_t index);
        uint16_t getUnsignedShort(int64_t index);
        uint32_t getUnignedInt(int64_t index);
        float getFloat(int64_t index);
        // TODO: getVector2D(int64_t index);
        // TODO: getVector3D(int64_t index);
        // TODO: getVector4D(int64_t index);
        // TODO: getMatrix2x2(int64_t index);
        // TODO: getMatrix3x3(int64_t index);
        // TODO: getMatrix4x4(int64_t index);
        bool isSparse();
        void makeSparse(BufferViewAccessorSparse* sparseAccessor);
        void makeNotSparse();
        static Type mapTypeNameToType(std::string typeName);
    };
}

#endif