#include "BufferViewAccessorCodec.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessorSparse.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
    void BufferViewAccessorCodec::fromGLTF(
        mitevox::BufferViewAccessor* bufferViewAccessorResult, 
        JSON* accessorJSON,
        safety::SafeArray<mitevox::BufferView*>* bufferViews)
    {
        bufferViewAccessorResult->name = accessorJSON->getFieldString("name");
        bufferViewAccessorResult->byteOffset = (uint64_t)accessorJSON->getFieldNumber("byteOffset");
        bufferViewAccessorResult->componentType = 
            (mitevox::ComponentDataType)accessorJSON->getFieldNumber("componentType");
        bufferViewAccessorResult->count = (uint64_t)accessorJSON->getFieldNumber("count");
        bufferViewAccessorResult->normalized = accessorJSON->getFieldBoolean("normalized");
        
        JSON* numberJSON = accessorJSON->getField("bufferView");
        if (numberJSON != nullptr)
        {
            if (numberJSON->isNumber())
            {
                int32_t bufferViewIndex = (int32_t)numberJSON->getNumber();
                bufferViewAccessorResult->bufferView = bufferViews->getElement(bufferViewIndex);
            }
        }

        std::string typeName = accessorJSON->getFieldString("type");
        bufferViewAccessorResult->type = mitevox::BufferViewAccessor::mapTypeNameToType(typeName);

        collectMinMaxFromGLTF(bufferViewAccessorResult, accessorJSON);

        JSON* accessorSparseJSON = accessorJSON->getField("sparse");
        if (accessorSparseJSON != nullptr)
        {
            sparseFromGLTF(bufferViewAccessorResult, accessorSparseJSON);
        }
    }

    void BufferViewAccessorCodec::collectMinMaxFromGLTF(
        mitevox::BufferViewAccessor* bufferViewAccessor, 
        JSON* accessorJSON)
    {
        JSON* minArrayJSON = accessorJSON->getFieldArray("min");
        if (minArrayJSON != nullptr)
        {
            minArrayJSON->toNumberArray<float>(&bufferViewAccessor->min);
        }

        JSON* maxArrayJSON = accessorJSON->getFieldArray("max");
        if (maxArrayJSON != nullptr)
        {
            maxArrayJSON->toNumberArray<float>(&bufferViewAccessor->max);
        }
    }

    void BufferViewAccessorCodec::sparseFromGLTF(
        mitevox::BufferViewAccessor* bufferViewAccessor,
        JSON* accessorSparseJSON)
    {
        mitevox::BufferViewAccessorSparse* bufferViewAccessorSparse = new mitevox::BufferViewAccessorSparse();
        JSON* numberJSON = accessorSparseJSON->getField("count");
        if (numberJSON != nullptr)
        {
            if (numberJSON->isNumber())
            {
                bufferViewAccessorSparse->count = (uint64_t)numberJSON->getNumber();
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
                    bufferViewAccessorSparse->indices.bufferViewIndex = (int32_t)numberJSON->getNumber();
                }
            }
            numberJSON = indicesJSON->getField("byteOffset");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    bufferViewAccessorSparse->indices.byteOffset = (uint64_t)numberJSON->getNumber();
                }
            }
            numberJSON = indicesJSON->getField("componentType");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    bufferViewAccessorSparse->indices.componentType =
                        (mitevox::ComponentDataType)numberJSON->getNumber();
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
                    bufferViewAccessorSparse->values.bufferViewIndex = (int32_t)numberJSON->getNumber();
                }
            }
            numberJSON = valuesJSON->getField("byteOffset");
            if (numberJSON != nullptr)
            {
                if (numberJSON->isNumber())
                {
                    bufferViewAccessorSparse->values.byteOffset = (uint64_t)numberJSON->getNumber();
                }
            }
        }

        bufferViewAccessor->makeSparse(bufferViewAccessorSparse);
    }
}