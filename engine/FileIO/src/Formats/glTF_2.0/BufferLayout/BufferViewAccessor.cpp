#include "BufferViewAccessor.h"
#include "BufferViewAccessorSparse.h"

namespace fileio
{
    BufferViewAccessor::BufferViewAccessor() :
        bufferView(nullptr),
        byteOffset(0),
        componentType(ComponentDataType::FLOAT),
        count(0),
        normalized(false),
        sparse(nullptr)
    {
        min = new safety::SafeFloatArray();
        max = new safety::SafeFloatArray();
    }

    void BufferViewAccessor::fromGLTF(JSON* accessorJSON, safety::SafeArray<BufferView*>* bufferViews)
    {
        name = accessorJSON->getFieldString("name");
        byteOffset = (uint64_t)accessorJSON->getFieldNumber("byteOffset");
        componentType = (ComponentDataType)accessorJSON->getFieldNumber("componentType");
        count = (uint64_t)accessorJSON->getFieldNumber("count");
        normalized = accessorJSON->getFieldBoolean("normalized");
        
        JSON* numberJSON = accessorJSON->getField("bufferView");
        if (numberJSON != nullptr)
        {
            if (numberJSON->isNumber())
            {
                int32_t bufferViewIndex = (int32_t)numberJSON->getNumber();
                bufferView = bufferViews->getElement(bufferViewIndex);
            }
        }

        std::string typeName = accessorJSON->getFieldString("type");
        type = mapTypeNameToType(typeName);

        collectMinMaxFromGLTF(accessorJSON);

        JSON* accessorSparseJSON = accessorJSON->getField("sparse");
        if (accessorSparseJSON != nullptr)
        {
            BufferViewAccessorSparse* accessorSparse = new BufferViewAccessorSparse();
            accessorSparse->fromGLTF(accessorSparseJSON);
            makeSparse(accessorSparse);
        }
    }

    void BufferViewAccessor::deallocate()
    {
        delete min;
        delete max;
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

    void BufferViewAccessor::collectMinMaxFromGLTF(fileio::JSON* accessorJSON)
    {
        JSON* minArrayJSON = accessorJSON->getFieldArray("min");
        if (minArrayJSON != nullptr)
        {
            minArrayJSON->toNumberArray<float>(min);
        }

        JSON* maxArrayJSON = accessorJSON->getFieldArray("max");
        if (maxArrayJSON != nullptr)
        {
            maxArrayJSON->toNumberArray<float>(max);
        }
    }
}