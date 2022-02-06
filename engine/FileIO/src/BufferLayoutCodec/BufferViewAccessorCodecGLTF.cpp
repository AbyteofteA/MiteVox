#include "BufferViewAccessorCodec.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessorSparse.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
    mitevox::BufferViewAccessor* BufferViewAccessorCodec::fromGLTF(
        JSON* accessorJSON,
        safety::SafeArray<mitevox::BufferView*>* bufferViews)
    {
        mitevox::BufferViewAccessor* bufferViewAccessor = new mitevox::BufferViewAccessor();
        bufferViewAccessor->name = accessorJSON->getFieldString("name");
        bufferViewAccessor->byteOffset = (uint64_t)accessorJSON->getFieldNumber("byteOffset");
        bufferViewAccessor->componentType = 
            (mitevox::ComponentDataType)accessorJSON->getFieldNumber("componentType");
        bufferViewAccessor->count = (uint64_t)accessorJSON->getFieldNumber("count");
        bufferViewAccessor->normalized = accessorJSON->getFieldBoolean("normalized");
        
        JSON* numberJSON = accessorJSON->getField("bufferView");
        if (numberJSON != nullptr)
        {
            if (numberJSON->isNumber())
            {
                int32_t bufferViewIndex = (int32_t)numberJSON->getNumber();
                bufferViewAccessor->bufferView = bufferViews->getElement(bufferViewIndex);
            }
        }

        std::string typeName = accessorJSON->getFieldString("type");
        bufferViewAccessor->type = mitevox::BufferViewAccessor::mapTypeNameToType(typeName);

        collectMinMaxFromGLTF(accessorJSON, bufferViewAccessor);

        JSON* accessorSparseJSON = accessorJSON->getField("sparse");
        if (accessorSparseJSON != nullptr)
        {
            sparseFromGLTF(accessorSparseJSON, bufferViewAccessor);
        }

        return bufferViewAccessor;
    }

    void BufferViewAccessorCodec::collectMinMaxFromGLTF(
        JSON* accessorJSON, 
        mitevox::BufferViewAccessor* bufferViewAccessor)
    {
        JSON* minArrayJSON = accessorJSON->getFieldArray("min");
        if (minArrayJSON != nullptr)
        {
            minArrayJSON->toNumberArray<float>(bufferViewAccessor->min);
        }

        JSON* maxArrayJSON = accessorJSON->getFieldArray("max");
        if (maxArrayJSON != nullptr)
        {
            maxArrayJSON->toNumberArray<float>(bufferViewAccessor->max);
        }
    }

    void BufferViewAccessorCodec::sparseFromGLTF(
        JSON* accessorSparseJSON, 
        mitevox::BufferViewAccessor* bufferViewAccessor)
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