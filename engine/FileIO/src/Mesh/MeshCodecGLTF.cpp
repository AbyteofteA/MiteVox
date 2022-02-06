#include "MeshCodec.h"

namespace fileio
{
	mitevox::Mesh* MeshCodec::fromGLTF(
		JSON* meshJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<Material*>* materials)
	{
		mitevox::Mesh* mesh = new mitevox::Mesh();
		mesh->name = meshJSON->getFieldString("name");

		JSON* weightsArrayJSON = meshJSON->getFieldArray("weights");
		if (weightsArrayJSON != nullptr)
		{
			weightsArrayJSON->toNumberArray<float>(&mesh->weights);
		}

		JSON* meshPrimitivesArrayJSON = meshJSON->getFieldArray("primitives");
		if (meshPrimitivesArrayJSON != nullptr)
		{
			size_t meshPrimitivesArraySize = meshPrimitivesArrayJSON->getArraySize();
			mesh->primitives.resize(meshPrimitivesArraySize);

			for (size_t i = 0; i < meshPrimitivesArraySize; ++i)
			{
				JSON* meshPrimitiveJSON = meshPrimitivesArrayJSON->getArrayItem(i);
				mitevox::MeshPrimitive* meshPrimitive = 
					meshPrimitiveFromGLTF(meshPrimitiveJSON, accessors, materials);
				mesh->primitives.setElement(i, meshPrimitive);
			}
		}
		
		return mesh;
	}

	mitevox::MeshPrimitive* MeshCodec::meshPrimitiveFromGLTF(
		JSON* meshPrimitiveJSON,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
		safety::SafeArray<Material*>* materials)
	{
		mitevox::MeshPrimitive* meshPrimitive = new mitevox::MeshPrimitive();
		JSON* meshAttributesJSON = meshPrimitiveJSON->getFieldObject("attributes");
		collectAttributesFromJSON(meshAttributesJSON, &meshPrimitive->attributes, accessors);

		JSON* numberJSON = meshPrimitiveJSON->getField("indices");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t indecesAccessorIndex = (int32_t)numberJSON->getNumber();
				meshPrimitive->indecesAccessor = accessors->getElement(indecesAccessorIndex);
			}
		}

		numberJSON = meshPrimitiveJSON->getField("material");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t materialIndex = (int32_t)numberJSON->getNumber();
				meshPrimitive->material = materials->getElement(materialIndex);
			}
		}
		else
		{
			meshPrimitive->material = new Material();
			materials->appendElement(meshPrimitive->material);
		}
		numberJSON = meshPrimitiveJSON->getField("mode");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				meshPrimitive->mode = (mitevox::MeshPrimitive::TopologyType)numberJSON->getNumber();
			}
		}

		JSON* targetsArrayJSON = meshPrimitiveJSON->getFieldArray("targets");
		if (targetsArrayJSON != nullptr)
		{
			size_t targetsArraySize = (size_t)targetsArrayJSON->getArraySize();
			meshPrimitive->morphTargets.resize(targetsArraySize);

			for (size_t i = 0; i < targetsArraySize; ++i)
			{
				mitevox::MeshAttributeSet* attributeSet = new mitevox::MeshAttributeSet();
				meshPrimitive->morphTargets.setElement(i, attributeSet);
				JSON* targetJSON = targetsArrayJSON->getArrayItem(i);
				collectAttributesFromJSON(targetJSON, attributeSet, accessors);
			}
		}

		return meshPrimitive;
	}

	void MeshCodec::collectAttributesFromJSON(
		JSON* meshAttributesJSON,
		mitevox::MeshAttributeSet* meshAttributeSet,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors)
	{
		JSON* numberJSON = meshAttributesJSON->getField("POSITION");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t positionAccessorIndex = (int32_t)numberJSON->getNumber();
				meshAttributeSet->positionAccessor = accessors->getElement(positionAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("NORMAL");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t normalAccessorIndex = (int32_t)numberJSON->getNumber();
				meshAttributeSet->normalAccessor = accessors->getElement(normalAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("TANGENT");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t tangentAccessorIndex = (int32_t)numberJSON->getNumber();
				meshAttributeSet->tangentAccessor = accessors->getElement(tangentAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("TEXCOORD_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t textureCoordAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->textureCoordAccessor_0 = accessors->getElement(textureCoordAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("TEXCOORD_1");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t textureCoordAccessorIndex_1 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->textureCoordAccessor_1 = accessors->getElement(textureCoordAccessorIndex_1);
			}
		}
		numberJSON = meshAttributesJSON->getField("COLOR_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t colorAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->colorAccessor_0 = accessors->getElement(colorAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("JOINTS_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t jointsAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->jointsAccessor_0 = accessors->getElement(jointsAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("WEIGHTS_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t weightsAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				meshAttributeSet->weightsAccessor_0 = accessors->getElement(weightsAccessorIndex_0);
			}
		}
	}
}