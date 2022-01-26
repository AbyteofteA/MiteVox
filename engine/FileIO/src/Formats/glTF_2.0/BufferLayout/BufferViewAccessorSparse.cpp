#include "BufferViewAccessorSparse.h"

namespace fileio
{
    BufferViewAccessorSparse::BufferViewAccessorSparse() :
        count(-1),
        indices({ -1, 0, ComponentDataType::FLOAT }),
        values({ -1, 0 })
    {

    }

    void BufferViewAccessorSparse::fromGLTF(JSON* accessorSparseJSON)
    {
        JSON* numberJSON = accessorSparseJSON->getField("count");
        if (numberJSON != nullptr)
        {
            if (numberJSON->isNumber())
            {
                count = (uint64_t)numberJSON->getNumber();
            }
        }

        JSON* indicesJSON = accessorSparseJSON->getField("indices");
        if (indicesJSON != nullptr)
        {
            numberJSON = indicesJSON->getField("bufferView");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    indices.bufferViewIndex = (int32_t)numberJSON->getNumber();
                }
            }
            numberJSON = indicesJSON->getField("byteOffset");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    indices.byteOffset = (uint64_t)numberJSON->getNumber();
                }
            }
            numberJSON = indicesJSON->getField("componentType");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    indices.componentType = (ComponentDataType)numberJSON->getNumber();
                }
            }
        }

        JSON* valuesJSON = accessorSparseJSON->getField("values");
        if (valuesJSON != nullptr)
        {
            numberJSON = valuesJSON->getField("bufferView");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    values.bufferViewIndex = (int32_t)numberJSON->getNumber();
                }
            }
            numberJSON = valuesJSON->getField("byteOffset");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    values.byteOffset = (uint64_t)numberJSON->getNumber();
                }
            }
        }
    }
}