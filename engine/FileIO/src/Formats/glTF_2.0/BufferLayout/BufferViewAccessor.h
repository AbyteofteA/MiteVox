#ifndef GLTFACCESSOR_H
#define GLTFACCESSOR_H

#include "engine/FileIO/src/Formats/glTF_2.0/BufferLayout/BufferView.h"
#include "engine/FileIO/src/Formats/glTF_2.0/BufferLayout/BufferViewAccessorSparse.h"
#include "engine/FileIO/src/Formats/glTF_2.0/BufferLayout/ComponentDataType.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>
#include <string>

namespace fileio
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

        BufferView* bufferView;
        uint64_t byteOffset;
        ComponentDataType componentType;
        uint64_t count;
        bool normalized;
        Type type;
        std::string name;

        safety::SafeFloatArray* min;
        safety::SafeFloatArray* max;
        BufferViewAccessorSparse* sparse;

        BufferViewAccessor();
        void fromGLTF(JSON* accessorJSON, safety::SafeArray<BufferView*>* bufferViews);
        void deallocate();
        bool isSparse();
        void makeSparse(BufferViewAccessorSparse* sparseAccessor);
        void makeNotSparse();

    private:

        Type mapTypeNameToType(std::string typeName);
        void collectMinMaxFromGLTF(fileio::JSON* accessorJSON);
    };
}

#endif