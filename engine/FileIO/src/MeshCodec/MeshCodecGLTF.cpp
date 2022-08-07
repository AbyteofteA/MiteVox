#include "MeshCodec.h"
#include "engine/MiteVox/src/Material/Material.h"

namespace fileio
{
	void MeshCodec::fromGLTF(
		mitevox::Mesh* meshResult, 
		JSON* meshJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<mitevox::Material*>* materials)
	{
		meshResult->name = meshJSON->getFieldString("name");

		JSON* weightsArrayJSON = meshJSON->getFieldArray("weights");
		if (weightsArrayJSON != nullptr)
		{
			weightsArrayJSON->toNumberArray<float>(&meshResult->weights);
		}

		JSON* meshPrimitivesArrayJSON = meshJSON->getFieldArray("primitives");
		if (meshPrimitivesArrayJSON != nullptr)
		{
			size_t meshPrimitivesArraySize = meshPrimitivesArrayJSON->getArraySize();
			meshResult->primitives.resize(meshPrimitivesArraySize);

			for (size_t i = 0; i < meshPrimitivesArraySize; ++i)
			{
				JSON* meshPrimitiveJSON = meshPrimitivesArrayJSON->getArrayItem(i);
				mitevox::MeshPrimitive* meshPrimitive = new mitevox::MeshPrimitive();
				meshPrimitiveFromGLTF(meshPrimitive, meshPrimitiveJSON, accessors, materials);
				meshResult->primitives.setElement(i, meshPrimitive);
			}
		}
	}

	void MeshCodec::meshPrimitiveFromGLTF(
		mitevox::MeshPrimitive* meshPrimitiveResult, 
		JSON* meshPrimitiveJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<mitevox::Material*>* materials)
	{
		JSON* meshAttributesJSON = meshPrimitiveJSON->getFieldObject("attributes");
		collectAttributesFromJSON(&meshPrimitiveResult->attributes, meshAttributesJSON, accessors);

		JSON* numberJSON = meshPrimitiveJSON->getField("indices");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t indecesAccessorIndex = (int32_t)numberJSON->getNumber();
				meshPrimitiveResult->indecesAccessor = accessors->getElement(indecesAccessorIndex);
			}
		}

		numberJSON = meshPrimitiveJSON->getField("material");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t materialIndex = (int32_t)numberJSON->getNumber();
				meshPrimitiveResult->material = materials->getElement(materialIndex);
			}
		}
		else
		{
			meshPrimitiveResult->material = new mitevox::Material();
			materials->appendElement(meshPrimitiveResult->material);
		}
		numberJSON = meshPrimitiveJSON->getField("mode");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				meshPrimitiveResult->topologyType = (mitevox::TopologyType)numberJSON->getNumber();
			}
		}

		JSON* targetsArrayJSON = meshPrimitiveJSON->getFieldArray("targets");
		if (targetsArrayJSON != nullptr)
		{
			size_t targetsArraySize = (size_t)targetsArrayJSON->getArraySize();
			meshPrimitiveResult->morphTargets.resize(targetsArraySize);

			for (size_t i = 0; i < targetsArraySize; ++i)
			{
				mitevox::MeshAttributeSet* attributeSet = new mitevox::MeshAttributeSet();
				meshPrimitiveResult->morphTargets.setElement(i, attributeSet);
				JSON* targetJSON = targetsArrayJSON->getArrayItem(i);
				collectAttributesFromJSON(attributeSet, targetJSON, accessors);
			}
		}
	}

	void MeshCodec::collectAttributesFromJSON(
		mitevox::MeshAttributeSet* meshAttributeSet,
		JSON* meshAttributesJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors)
	{
		JSON* numberJSON = meshAttributesJSON->getField("POSITION");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t positionAccessorIndex = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.positionAccessor = accessors->getElement(positionAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("NORMAL");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t normalAccessorIndex = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.normalAccessor = accessors->getElement(normalAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("TANGENT");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t tangentAccessorIndex = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.tangentAccessor = accessors->getElement(tangentAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("TEXCOORD_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t textureCoordAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.textureCoordAccessor_0 = accessors->getElement(textureCoordAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("TEXCOORD_1");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t textureCoordAccessorIndex_1 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.textureCoordAccessor_1 = accessors->getElement(textureCoordAccessorIndex_1);
			}
		}
		numberJSON = meshAttributesJSON->getField("COLOR_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t colorAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.colorAccessor_0 = accessors->getElement(colorAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("JOINTS_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t jointsAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.jointsAccessor_0 = accessors->getElement(jointsAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("WEIGHTS_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t weightsAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->byName.weightsAccessor_0 = accessors->getElement(weightsAccessorIndex_0);
			}
		}
	}
}