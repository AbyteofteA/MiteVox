#ifndef GLTFACCESSOR_H
#define GLTFACCESSOR_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessorSparse.h"
#include "engine/MiteVox/src/BufferLayout/ComponentDataType.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Matrix.h"

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

        safety::SafeFloatArray min;
        safety::SafeFloatArray max;
        BufferViewAccessorSparse* sparse = nullptr;

        uint32_t getComponentsCount();

        template<typename T>
        T getElementsComponent(int64_t elementIndex, int64_t componentIndex);

        float getElementsComponentAsFloat(int64_t elementIndex, int64_t componentIndex);
        uint32_t getElementsComponentAsUint(int64_t elementIndex, int64_t componentIndex);
        int32_t getElementsComponentAsInt(int64_t elementIndex, int64_t componentIndex);

        mathem::Vector2D getVector2D(int64_t index);
        mathem::Vector3D getVector3D(int64_t index);
        mathem::Vector4D getVector4D(int64_t index);
        mathem::Matrix2x2 getMatrix2x2(int64_t index);
        mathem::Matrix3x3 getMatrix3x3(int64_t index);
        mathem::Matrix4x4 getMatrix4x4(int64_t index);

        bool isSparse();
        void makeSparse(BufferViewAccessorSparse* sparseAccessor);
        void makeNotSparse();
        static Type mapTypeNameToType(std::string typeName);

    private:

        void* getDataStart();
    };
}

#endif