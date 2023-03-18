#include "MeshCodec.h"
#include "engine/MiteVox/src/Material/Material.h"

namespace fileio
{
	void MeshCodec::fromGLTF(
		mitevox::Mesh* meshResult, 
		JSON* meshJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<mitevox::Material>* materials)
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

	void MeshCodec::toGLTF(
		JSON* meshJSONResult,
		mitevox::Mesh* mesh,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<mitevox::Material>* materials)
	{
		meshJSONResult->setField("name", mesh->name);

		if (mesh->isMorphable())
		{
			JSON* weightsArrayJSON = meshJSONResult->setFieldType("weights", JSONtype::ARRAY);
			weightsArrayJSON->fromNumberArray<float>(&mesh->weights);
		}

		JSON* meshPrimitivesArrayJSON = meshJSONResult->setFieldType("primitives", JSONtype::ARRAY);
		size_t meshPrimitivesArraySize = mesh->primitives.getElementsCount();
		for (size_t i = 0; i < meshPrimitivesArraySize; ++i)
		{
			JSON* meshPrimitiveJSON = new JSON();
			meshPrimitiveJSON->setType(JSONtype::OBJECT);
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(i);
			meshPrimitiveToGLTF(meshPrimitiveJSON, meshPrimitive, accessors, materials);
			meshPrimitivesArrayJSON->addArrayItem(meshPrimitiveJSON);
		}
	}

	void MeshCodec::meshPrimitiveFromGLTF(
		mitevox::MeshPrimitive* meshPrimitiveResult, 
		JSON* meshPrimitiveJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<mitevox::Material>* materials)
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
			meshPrimitiveResult->material = materials->getElementPointer(materialIndex);
		}
		else
		{
			materials->appendElement(mitevox::Material());
			meshPrimitiveResult->material = materials->getLastElementPointer();
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

	void MeshCodec::meshPrimitiveToGLTF(
		JSON* meshPrimitiveJSONResult,
		mitevox::MeshPrimitive* meshPrimitive,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<mitevox::Material>* materials)
	{
		JSON* meshAttributesJSON = meshPrimitiveJSONResult->setFieldType("attributes", JSONtype::OBJECT);
		saveAttributesToJSON(meshAttributesJSON, &meshPrimitive->attributes, accessors);

		size_t accessorsCount = accessors->getElementsCount();
		for (size_t i = 0; i < accessorsCount; ++i)
		{
			if (meshPrimitive->indecesAccessor == accessors->getElement(i))
			{
				meshPrimitiveJSONResult->setField("indices", (double)i);
				break;
			}
		}

		size_t materialsCount = accessors->getElementsCount();
		for (size_t i = 0; i < materialsCount; ++i)
		{
			if (meshPrimitive->material == materials->getElementPointer(i))
			{
				meshPrimitiveJSONResult->setField("material", (double)i);
				break;
			}
		}
		meshPrimitiveJSONResult->setField("mode", (double)meshPrimitive->topologyType);

		size_t morphTargetsCount = meshPrimitive->morphTargets.getElementsCount();
		if (morphTargetsCount > 0)
		{
			JSON* targetsArrayJSON = meshPrimitiveJSONResult->setFieldType("targets", JSONtype::ARRAY);
			for (size_t i = 0; i < morphTargetsCount; ++i)
			{
				mitevox::MeshAttributeSet* attributeSet = meshPrimitive->morphTargets.getElement(i);
				JSON* targetJSON = new JSON();
				targetJSON->setType(JSONtype::OBJECT);
				saveAttributesToJSON(targetJSON, attributeSet, accessors);
				targetsArrayJSON->addArrayItem(targetJSON);
			}
		}
	}

	void MeshCodec::saveAttributesToJSON(
		JSON* meshAttributesJSON,
		mitevox::MeshAttributeSet* meshAttributeSet,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors)
	{
		size_t accessorsCount = accessors->getElementsCount();

		if (meshAttributeSet->byName.positionAccessor)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.positionAccessor == accessors->getElement(i))
				{
					meshAttributesJSON->setField("POSITION", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.normalAccessor)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.normalAccessor == accessors->getElement(i))
				{
					meshAttributesJSON->setField("NORMAL", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.tangentAccessor)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.tangentAccessor == accessors->getElement(i))
				{
					meshAttributesJSON->setField("TANGENT", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.textureCoordAccessor_0)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.textureCoordAccessor_0 == accessors->getElement(i))
				{
					meshAttributesJSON->setField("TEXCOORD_0", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.textureCoordAccessor_1)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.textureCoordAccessor_1 == accessors->getElement(i))
				{
					meshAttributesJSON->setField("TEXCOORD_1", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.colorAccessor_0)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.colorAccessor_0 == accessors->getElement(i))
				{
					meshAttributesJSON->setField("COLOR_0", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.jointsAccessor_0)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.jointsAccessor_0 == accessors->getElement(i))
				{
					meshAttributesJSON->setField("JOINTS_0", (double)i);
					break;
				}
			}
		}
		if (meshAttributeSet->byName.weightsAccessor_0)
		{
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (meshAttributeSet->byName.weightsAccessor_0 == accessors->getElement(i))
				{
					meshAttributesJSON->setField("WEIGHTS_0", (double)i);
					break;
				}
			}
		}
	}
}