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
        bufferViewAccessorResult->name = accessorJSON->getFieldStringOrDefault("name", "Untitled");
        bufferViewAccessorResult->byteOffset = (uint64_t)accessorJSON->getFieldNumberOrDefault("byteOffset", 0.0);
        bufferViewAccessorResult->componentType = 
            (mitevox::ComponentDataType)accessorJSON->getFieldNumberOrDefault("componentType", (double)mitevox::ComponentDataType::FLOAT);
        bufferViewAccessorResult->count = (uint64_t)accessorJSON->getFieldNumberOrDefault("count", 0.0);
        bufferViewAccessorResult->normalized = accessorJSON->getFieldBooleanOrDefault("normalized", false);
        
        if (JSON* numberJSON = accessorJSON->getField("bufferView"))
        {
            int32_t bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
            bufferViewAccessorResult->bufferView = bufferViews->getElement(bufferViewIndex);
        }

        std::string typeName = accessorJSON->getFieldStringOrDefault("type", "SCALAR");
        bufferViewAccessorResult->type = mitevox::BufferViewAccessor::mapTypeNameToType(typeName);

        collectMinMaxFromGLTF(bufferViewAccessorResult, accessorJSON);

        if (JSON* accessorSparseJSON = accessorJSON->getField("sparse"))
        {
            sparseFromGLTF(bufferViewAccessorResult, accessorSparseJSON);
        }
    }

    void BufferViewAccessorCodec::collectMinMaxFromGLTF(
        mitevox::BufferViewAccessor* bufferViewAccessor, 
        JSON* accessorJSON)
    {
        if (JSON* minArrayJSON = accessorJSON->getFieldArray("min"))
        {
            minArrayJSON->toNumberArray<float>(&bufferViewAccessor->min);
        }

        if (JSON* maxArrayJSON = accessorJSON->getFieldArray("max"))
        {
            maxArrayJSON->toNumberArray<float>(&bufferViewAccessor->max);
        }
    }

    void BufferViewAccessorCodec::sparseFromGLTF(
        mitevox::BufferViewAccessor* bufferViewAccessor,
        JSON* accessorSparseJSON)
    {
        mitevox::BufferViewAccessorSparse* bufferViewAccessorSparse = new mitevox::BufferViewAccessorSparse();
        if (JSON* numberJSON = accessorSparseJSON->getField("count"))
        {
            bufferViewAccessorSparse->count = (uint64_t)numberJSON->getNumberOrDefault(0.0);
        }

        if (JSON* indicesJSON = accessorSparseJSON->getField("indices"))
        {
            if (JSON* numberJSON = indicesJSON->getField("bufferView"))
            {
                bufferViewAccessorSparse->indices.bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
            }
            if (JSON* numberJSON = indicesJSON->getField("byteOffset"))
            {
                bufferViewAccessorSparse->indices.byteOffset = (uint64_t)numberJSON->getNumberOrDefault(0.0);
            }
            if (JSON* numberJSON = indicesJSON->getField("componentType"))
            {
                bufferViewAccessorSparse->indices.componentType =
                    (mitevox::ComponentDataType)numberJSON->getNumberOrDefault((double)mitevox::ComponentDataType::UNSIGNED_SHORT);
            }
        }

        if (JSON* valuesJSON = accessorSparseJSON->getField("values"))
        {
            if (JSON* numberJSON = valuesJSON->getField("bufferView"))
            {
                bufferViewAccessorSparse->values.bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
            }
            if (JSON* numberJSON = valuesJSON->getField("byteOffset"))
            {
                bufferViewAccessorSparse->values.byteOffset = (uint64_t)numberJSON->getNumberOrDefault(0.0);
            }
        }

        bufferViewAccessor->makeSparse(bufferViewAccessorSparse);
    }
}