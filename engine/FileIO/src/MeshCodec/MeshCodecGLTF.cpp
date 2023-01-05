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
		meshResult->name = meshJSON->getFieldStringOrDefault("name", "Untitled");

		if (JSON* weightsArrayJSON = meshJSON->getFieldArray("weights"))
		{
			weightsArrayJSON->toNumberArray<float>(&meshResult->weights);
		}

		if (JSON* meshPrimitivesArrayJSON = meshJSON->getFieldArray("primitives"))
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

		if (JSON* numberJSON = meshPrimitiveJSON->getField("indices"))
		{
			int32_t indecesAccessorIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshPrimitiveResult->indecesAccessor = accessors->getElement(indecesAccessorIndex);
		}

		if (JSON* numberJSON = meshPrimitiveJSON->getField("material"))
		{
			int32_t materialIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshPrimitiveResult->material = materials->getElement(materialIndex);
		}
		else
		{
			meshPrimitiveResult->material = new mitevox::Material();
			materials->appendElement(meshPrimitiveResult->material);
		}
		if (JSON* numberJSON = meshPrimitiveJSON->getField("mode"))
		{
			meshPrimitiveResult->topologyType = (mitevox::TopologyType)numberJSON->getNumberOrDefault((double)mitevox::TopologyType::TRIANGLES);
		}

		if (JSON* targetsArrayJSON = meshPrimitiveJSON->getFieldArray("targets"))
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
		if (JSON* numberJSON = meshAttributesJSON->getField("POSITION"))
		{
			int32_t positionAccessorIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.positionAccessor = accessors->getElement(positionAccessorIndex);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("NORMAL"))
		{
			int32_t normalAccessorIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.normalAccessor = accessors->getElement(normalAccessorIndex);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("TANGENT"))
		{
			int32_t tangentAccessorIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.tangentAccessor = accessors->getElement(tangentAccessorIndex);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("TEXCOORD_0"))
		{
			int32_t textureCoordAccessorIndex_0 = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.textureCoordAccessor_0 = accessors->getElement(textureCoordAccessorIndex_0);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("TEXCOORD_1"))
		{
			int32_t textureCoordAccessorIndex_1 = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.textureCoordAccessor_1 = accessors->getElement(textureCoordAccessorIndex_1);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("COLOR_0"))
		{
			int32_t colorAccessorIndex_0 = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.colorAccessor_0 = accessors->getElement(colorAccessorIndex_0);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("JOINTS_0"))
		{
			int32_t jointsAccessorIndex_0 = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.jointsAccessor_0 = accessors->getElement(jointsAccessorIndex_0);
		}
		if (JSON* numberJSON = meshAttributesJSON->getField("WEIGHTS_0"))
		{
			int32_t weightsAccessorIndex_0 = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			meshAttributeSet->byName.weightsAccessor_0 = accessors->getElement(weightsAccessorIndex_0);
		}
	}
}