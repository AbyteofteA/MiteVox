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
        bufferViewAccessorResult->byteOffset = (uint64_t)accessorJSON->getFieldNumberOrDefault("byteOffset", 0.0f);
        bufferViewAccessorResult->componentType = 
            (mitevox::ComponentDataType)accessorJSON->getFieldNumberOrDefault("componentType", (double)mitevox::ComponentDataType::FLOAT);
        bufferViewAccessorResult->count = (uint64_t)accessorJSON->getFieldNumberOrDefault("count", 0.0f);
        bufferViewAccessorResult->normalized = accessorJSON->getFieldBooleanOrDefault("normalized", false);
        
        if (JSON* numberJSON = accessorJSON->getField("bufferView"))
        {
            int32_t bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
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

    void BufferViewAccessorCodec::toGLTF(
        JSON* accessorJSONResult,
        mitevox::BufferViewAccessor* bufferViewAccessor,
        safety::SafeArray<mitevox::BufferView*>* bufferViews)
    {
        accessorJSONResult->setField("name", bufferViewAccessor->name);
        accessorJSONResult->setField("byteOffset", (double)bufferViewAccessor->byteOffset);
        accessorJSONResult->setField("componentType", (double)bufferViewAccessor->componentType);
        accessorJSONResult->setField("count", (double)bufferViewAccessor->count);
        accessorJSONResult->setField("normalized", bufferViewAccessor->normalized);
        accessorJSONResult->setField("type", mitevox::BufferViewAccessor::mapTypeToTypeName(bufferViewAccessor->type));

        size_t bufferViewsCount = bufferViews->getElementsCount();
        for (size_t i = 0; i < bufferViewsCount; ++i)
        {
            if (bufferViewAccessor->bufferView == bufferViews->getElement(i))
            {
                accessorJSONResult->setField("bufferView", (double)i);
                break;
            }
        }

        saveMinMaxToGLTF(accessorJSONResult, bufferViewAccessor);

        if (bufferViewAccessor->isSparse())
        {
            JSON* accessorSparseJSON = accessorJSONResult->setFieldType("sparse", JSONtype::OBJECT);
            sparseToGLTF(accessorSparseJSON, bufferViewAccessor);
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
            bufferViewAccessorSparse->count = (uint64_t)numberJSON->getNumberOrDefault(0.0f);
        }

        if (JSON* indicesJSON = accessorSparseJSON->getField("indices"))
        {
            if (JSON* numberJSON = indicesJSON->getField("bufferView"))
            {
                bufferViewAccessorSparse->indices.bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
            }
            if (JSON* numberJSON = indicesJSON->getField("byteOffset"))
            {
                bufferViewAccessorSparse->indices.byteOffset = (uint64_t)numberJSON->getNumberOrDefault(0.0f);
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
                bufferViewAccessorSparse->values.bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
            }
            if (JSON* numberJSON = valuesJSON->getField("byteOffset"))
            {
                bufferViewAccessorSparse->values.byteOffset = (uint64_t)numberJSON->getNumberOrDefault(0.0f);
            }
        }

        bufferViewAccessor->makeSparse(bufferViewAccessorSparse);
    }

    void BufferViewAccessorCodec::saveMinMaxToGLTF(JSON* accessorJSON, mitevox::BufferViewAccessor* bufferViewAccessor)
    {
        JSON* minArrayJSON = accessorJSON->setFieldType("min", JSONtype::ARRAY);
        minArrayJSON->fromNumberArray<float>(&bufferViewAccessor->min);
        JSON* maxArrayJSON = accessorJSON->setFieldType("max", JSONtype::ARRAY);
        maxArrayJSON->fromNumberArray<float>(&bufferViewAccessor->max);
    }

    void BufferViewAccessorCodec::sparseToGLTF(JSON* accessorSparseJSON, mitevox::BufferViewAccessor* bufferViewAccessor)
    {
        accessorSparseJSON->setField("count", (double)bufferViewAccessor->sparse->count);

        JSON* indicesJSON = accessorSparseJSON->setFieldType("indices", JSONtype::OBJECT);
        indicesJSON->setField("bufferView", (double)bufferViewAccessor->sparse->indices.bufferViewIndex);
        indicesJSON->setField("byteOffset", (double)bufferViewAccessor->sparse->indices.byteOffset);
        indicesJSON->setField("componentType", (double)bufferViewAccessor->sparse->indices.componentType);

        JSON* valuesJSON = accessorSparseJSON->setFieldType("values", JSONtype::OBJECT);
        valuesJSON->setField("bufferView", (double)bufferViewAccessor->sparse->values.bufferViewIndex);
        valuesJSON->setField("byteOffset", (double)bufferViewAccessor->sparse->values.byteOffset);
    }

}