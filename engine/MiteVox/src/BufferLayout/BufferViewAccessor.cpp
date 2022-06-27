#include "BufferViewAccessor.h"

namespace mitevox
{
    uint32_t BufferViewAccessor::getComponentsCount()
    {
        switch (type)
        {
        case mitevox::BufferViewAccessor::Type::SCALAR:
            return 1;
        case mitevox::BufferViewAccessor::Type::VEC2:
            return 2;
        case mitevox::BufferViewAccessor::Type::VEC3:
            return 3;
        case mitevox::BufferViewAccessor::Type::VEC4:
            return 4;
        case mitevox::BufferViewAccessor::Type::MAT2:
            return 4;
        case mitevox::BufferViewAccessor::Type::MAT3:
            return 9;
        case mitevox::BufferViewAccessor::Type::MAT4:
            return 16;
        default:
            return 0;
        }
    }

    template <typename T>
    T BufferViewAccessor::getElementsComponent(int64_t elementIndex, int64_t componentIndex)
    {
        if (elementIndex >= count ||
            componentIndex >= getComponentsCount())
        {
            return (T)0;
        }
        int64_t actualIndex = 0;
        if (bufferView->byteStride == 0)
        {
            actualIndex = elementIndex * getComponentsCount() + componentIndex;
        }
        else
        {
            actualIndex = elementIndex * bufferView->byteStride + componentIndex;
        }
        return ((T*)getDataStart())[actualIndex];
    }

    float BufferViewAccessor::getElementsComponentAsFloat(int64_t elementIndex, int64_t componentIndex)
    {
        if (normalized)
        {
            switch (componentType)
            {
            case mitevox::BYTE:
            {
                float storedValue = (float)getElementsComponent<int8_t>(elementIndex, componentIndex);
                return std::max(storedValue / 127.0f, -1.0f);
            }
            case mitevox::UNSIGNED_BYTE:
            {
                float storedValue = (float)getElementsComponent<uint8_t>(elementIndex, componentIndex);
                return storedValue / 255.0f;
            }
            case mitevox::SHORT:
            {
                float storedValue = (float)getElementsComponent<int16_t>(elementIndex, componentIndex);
                return std::max(storedValue / 32767.0f, -1.0f);
            }
            case mitevox::UNSIGNED_SHORT:
            {
                float storedValue = (float)getElementsComponent<uint16_t>(elementIndex, componentIndex);
                return storedValue / 65535.0f;
            }
            case mitevox::UNSIGNED_INT:
                return (float)getElementsComponent<uint32_t>(elementIndex, componentIndex);
            case mitevox::FLOAT:
                return (float)getElementsComponent<float>(elementIndex, componentIndex);
            default:
                return 0.0f;
            }
        }
        else
        {
            switch (componentType)
            {
            case mitevox::BYTE:
                return (float)getElementsComponent<int8_t>(elementIndex, componentIndex);
            case mitevox::UNSIGNED_BYTE:
                return (float)getElementsComponent<uint8_t>(elementIndex, componentIndex);
            case mitevox::SHORT:
                return (float)getElementsComponent<int16_t>(elementIndex, componentIndex);
            case mitevox::UNSIGNED_SHORT:
                return (float)getElementsComponent<uint16_t>(elementIndex, componentIndex);
            case mitevox::UNSIGNED_INT:
                return (float)getElementsComponent<uint32_t>(elementIndex, componentIndex);
            case mitevox::FLOAT:
                return (float)getElementsComponent<float>(elementIndex, componentIndex);
            default:
                return 0.0f;
            }
        }
    }

    uint32_t BufferViewAccessor::getElementsComponentAsUint(int64_t elementIndex, int64_t componentIndex)
    {
        switch (componentType)
        {
        case mitevox::BYTE:
            return (uint32_t)getElementsComponent<int8_t>(elementIndex, componentIndex);
        case mitevox::UNSIGNED_BYTE:
            return (uint32_t)getElementsComponent<uint8_t>(elementIndex, componentIndex);
        case mitevox::SHORT:
            return (uint32_t)getElementsComponent<int16_t>(elementIndex, componentIndex);
        case mitevox::UNSIGNED_SHORT:
            return (uint32_t)getElementsComponent<uint16_t>(elementIndex, componentIndex);
        case mitevox::UNSIGNED_INT:
            return (uint32_t)getElementsComponent<uint32_t>(elementIndex, componentIndex);
        case mitevox::FLOAT:
            return (uint32_t)getElementsComponent<float>(elementIndex, componentIndex);
        default:
            return 0;
        }
    }

    int32_t BufferViewAccessor::getElementsComponentAsInt(int64_t elementIndex, int64_t componentIndex)
    {
        switch (componentType)
        {
        case mitevox::BYTE:
            return (int32_t)getElementsComponent<int8_t>(elementIndex, componentIndex);
        case mitevox::UNSIGNED_BYTE:
            return (int32_t)getElementsComponent<uint8_t>(elementIndex, componentIndex);
        case mitevox::SHORT:
            return (int32_t)getElementsComponent<int16_t>(elementIndex, componentIndex);
        case mitevox::UNSIGNED_SHORT:
            return (int32_t)getElementsComponent<uint16_t>(elementIndex, componentIndex);
        case mitevox::UNSIGNED_INT:
            return (int32_t)getElementsComponent<uint32_t>(elementIndex, componentIndex);
        case mitevox::FLOAT:
            return (int32_t)getElementsComponent<float>(elementIndex, componentIndex);
        default:
            return 0;
        }
    }

    void BufferViewAccessor::getElementsComponentsAsFloatArray(
        safety::SafeFloatArray* resultArray, 
        int64_t elementIndex, 
        int64_t componentsCount)
    {
        for (size_t i = 0; i < componentsCount; ++i)
        {
            float value = getElementsComponentAsFloat(i + elementIndex * componentsCount, 0);
            resultArray->setElement(i, value);
        }
    }

    template <typename T>
    void BufferViewAccessor::setElementsComponent(int64_t elementIndex, int64_t componentIndex, T value)
    {
        if (elementIndex >= count ||
            componentIndex >= getComponentsCount())
        {
            return;
        }
        int64_t actualIndex = 0;
        if (bufferView->byteStride == 0)
        {
            actualIndex = elementIndex * getComponentsCount() + componentIndex;
        }
        else
        {
            actualIndex = elementIndex * bufferView->byteStride + componentIndex;
        }
        ((T*)getDataStart())[actualIndex] = value;
    }

    void BufferViewAccessor::setElementsComponentAsFloat(int64_t elementIndex, int64_t componentIndex, float value)
    {
        if (normalized)
        {
            switch (componentType)
            {
            case mitevox::BYTE:
            {
                int8_t normalizedValue = std::round(value * 127.0);
                setElementsComponent<int8_t>(elementIndex, componentIndex, normalizedValue);
                break;
            }
            case mitevox::UNSIGNED_BYTE:
            {
                uint8_t normalizedValue = std::round(value * 255.0);
                setElementsComponent<uint8_t>(elementIndex, componentIndex, normalizedValue);
                break;
            }
            case mitevox::SHORT:
            {
                int16_t normalizedValue = std::round(value * 32767.0);
                setElementsComponent<int16_t>(elementIndex, componentIndex, normalizedValue);
                break;
            }
            case mitevox::UNSIGNED_SHORT:
            {
                uint16_t normalizedValue = std::round(value * 65535.0);
                setElementsComponent<uint16_t>(elementIndex, componentIndex, normalizedValue);
                break;
            }
            case mitevox::UNSIGNED_INT:
                setElementsComponent<uint32_t>(elementIndex, componentIndex, (uint32_t)value); break;
            case mitevox::FLOAT:
                setElementsComponent<float>(elementIndex, componentIndex, value); break;
            default: break;
            }
        }
        else
        {
            switch (componentType)
            {
            case mitevox::BYTE:
                setElementsComponent<int8_t>(elementIndex, componentIndex, (int8_t)value); break;
            case mitevox::UNSIGNED_BYTE:
                setElementsComponent<uint8_t>(elementIndex, componentIndex, (uint8_t)value); break;
            case mitevox::SHORT:
                setElementsComponent<int16_t>(elementIndex, componentIndex, (int16_t)value); break;
            case mitevox::UNSIGNED_SHORT:
                setElementsComponent<uint16_t>(elementIndex, componentIndex, (uint16_t)value); break;
            case mitevox::UNSIGNED_INT:
                setElementsComponent<uint32_t>(elementIndex, componentIndex, (uint32_t)value); break;
            case mitevox::FLOAT:
                setElementsComponent<float>(elementIndex, componentIndex, value); break;
            default: break;
            }
        }
    }

    mathem::Vector2D BufferViewAccessor::getVector2D(int64_t index)
    {
        mathem::Vector2D resultVector2D;
        resultVector2D.u() = getElementsComponentAsFloat(index, 0);
        resultVector2D.v() = getElementsComponentAsFloat(index, 1);
        return resultVector2D;
    }

    mathem::Vector3D BufferViewAccessor::getVector3D(int64_t index)
    {
        mathem::Vector3D resultVector3D;
        resultVector3D.x() = getElementsComponentAsFloat(index, 0);
        resultVector3D.y() = getElementsComponentAsFloat(index, 1);
        resultVector3D.z() = getElementsComponentAsFloat(index, 2);
        return resultVector3D;
    }

    mathem::Vector4D BufferViewAccessor::getVector4D(int64_t index)
    {
        mathem::Vector4D resultVector4D;
        resultVector4D.x() = getElementsComponentAsFloat(index, 0);
        resultVector4D.y() = getElementsComponentAsFloat(index, 1);
        resultVector4D.z() = getElementsComponentAsFloat(index, 2);
        resultVector4D.s() = getElementsComponentAsFloat(index, 3);
        return resultVector4D;
    }

    mathem::Matrix2x2 BufferViewAccessor::getMatrix2x2(int64_t index)
    {
        mathem::Matrix2x2 resultMatrix2x2;
        resultMatrix2x2.at(0, 0) = getElementsComponentAsFloat(index, 0);
        resultMatrix2x2.at(0, 1) = getElementsComponentAsFloat(index, 1);
        resultMatrix2x2.at(1, 0) = getElementsComponentAsFloat(index, 2);
        resultMatrix2x2.at(1, 1) = getElementsComponentAsFloat(index, 3);
        return resultMatrix2x2;
    }

    mathem::Matrix3x3 BufferViewAccessor::getMatrix3x3(int64_t index)
    {
        mathem::Matrix3x3 resultMatrix3x3;
        resultMatrix3x3.at(0, 0) = getElementsComponentAsFloat(index, 0);
        resultMatrix3x3.at(0, 1) = getElementsComponentAsFloat(index, 1);
        resultMatrix3x3.at(0, 2) = getElementsComponentAsFloat(index, 2);
        resultMatrix3x3.at(1, 0) = getElementsComponentAsFloat(index, 3);
        resultMatrix3x3.at(1, 1) = getElementsComponentAsFloat(index, 4);
        resultMatrix3x3.at(1, 2) = getElementsComponentAsFloat(index, 5);
        resultMatrix3x3.at(2, 0) = getElementsComponentAsFloat(index, 6);
        resultMatrix3x3.at(2, 1) = getElementsComponentAsFloat(index, 7);
        resultMatrix3x3.at(2, 2) = getElementsComponentAsFloat(index, 8);
        return resultMatrix3x3;
    }

    mathem::Matrix4x4 BufferViewAccessor::getMatrix4x4(int64_t index)
    {
        mathem::Matrix4x4 resultMatrix4x4;
        for (uint32_t indexX = 0; indexX < 4; ++indexX)
        {
            for (uint32_t indexY = 0; indexY < 4; ++indexY)
            {
                size_t actualIndex = indexX * 4 + indexY;
                resultMatrix4x4.at(indexX, indexY) = getElementsComponentAsFloat(index, actualIndex);
            }
        }
        return resultMatrix4x4;
    }

    void BufferViewAccessor::setVector2D(int64_t index, mathem::Vector2D value)
    {
        setElementsComponentAsFloat(index, 0, value.u());
        setElementsComponentAsFloat(index, 1, value.v());
    }

    void BufferViewAccessor::setVector3D(int64_t index, mathem::Vector3D value)
    {
        setElementsComponentAsFloat(index, 0, value.x());
        setElementsComponentAsFloat(index, 1, value.y());
        setElementsComponentAsFloat(index, 2, value.z());
    }

    void BufferViewAccessor::setVector4D(int64_t index, mathem::Vector4D value)
    {
        setElementsComponentAsFloat(index, 0, value.x());
        setElementsComponentAsFloat(index, 1, value.y());
        setElementsComponentAsFloat(index, 2, value.z());
        setElementsComponentAsFloat(index, 3, value.s());
    }

    void BufferViewAccessor::setMatrix2x2(int64_t index, mathem::Matrix2x2 value)
    {
        setElementsComponentAsFloat(index, 0, value.at(0, 0));
        setElementsComponentAsFloat(index, 1, value.at(0, 1));
        setElementsComponentAsFloat(index, 2, value.at(1, 0));
        setElementsComponentAsFloat(index, 3, value.at(1, 1));
    }

    void BufferViewAccessor::setMatrix3x3(int64_t index, mathem::Matrix3x3 value)
    {
        setElementsComponentAsFloat(index, 0, value.at(0, 0));
        setElementsComponentAsFloat(index, 1, value.at(0, 1));
        setElementsComponentAsFloat(index, 2, value.at(0, 2));
        setElementsComponentAsFloat(index, 3, value.at(1, 0));
        setElementsComponentAsFloat(index, 4, value.at(1, 1));
        setElementsComponentAsFloat(index, 5, value.at(1, 2));
        setElementsComponentAsFloat(index, 6, value.at(2, 0));
        setElementsComponentAsFloat(index, 7, value.at(2, 1));
        setElementsComponentAsFloat(index, 8, value.at(2, 2));
    }

    void BufferViewAccessor::setMatrix4x4(int64_t index, mathem::Matrix4x4 value)
    {
        for (uint32_t indexX = 0; indexX < 4; ++indexX)
        {
            for (uint32_t indexY = 0; indexY < 4; ++indexY)
            {
                size_t actualIndex = indexX * 4 + indexY;
                setElementsComponentAsFloat(index, actualIndex, value.at(indexX, indexY));
            }
        }
    }

    bool BufferViewAccessor::isSparse()
    {
        return sparse != nullptr;
    }

    void BufferViewAccessor::makeSparse(BufferViewAccessorSparse* sparseAccessor)
    {
        sparse = sparseAccessor;
    }

    void BufferViewAccessor::makeNotSparse()
    {
        delete sparse;
        sparse = nullptr;
    }

    BufferViewAccessor::Type BufferViewAccessor::mapTypeNameToType(std::string typeName)
    {
        if (typeName == "SCALAR")
        {
            return Type::SCALAR;
        }
        else if (typeName == "VEC2")
        {
            return Type::VEC2;
        }
        else if (typeName == "VEC3")
        {
            return Type::VEC3;
        }
        else if (typeName == "VEC4")
        {
            return Type::VEC4;
        }
        else if (typeName == "MAT2")
        {
            return Type::MAT2;
        }
        else if (typeName == "MAT3")
        {
            return Type::MAT3;
        }
        else if (typeName == "MAT4")
        {
            return Type::MAT4;
        }

        return Type::INVALID;
    }

    void BufferViewAccessor::makeSeparateCopyTo(BufferViewAccessor* resultBufferViewAccessor)
    {
        *resultBufferViewAccessor = *this;
        if (this->bufferView)
        { 
            resultBufferViewAccessor->bufferView = new BufferView();
            this->bufferView->makeSeparateCopyTo(resultBufferViewAccessor->bufferView);
        }
        if (this->sparse)
        {
            // TODO: copy sparse
        }
    }

    void* BufferViewAccessor::getDataStart()
    {
        return (uint8_t*)bufferView->getDataStart() + byteOffset;
    }
}