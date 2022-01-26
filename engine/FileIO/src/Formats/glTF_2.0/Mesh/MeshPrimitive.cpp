#include "MeshPrimitive.h"


namespace fileio
{
	MeshPrimitive::MeshPrimitive() :
		indecesAccessor(nullptr),
		material(nullptr),
		mode(TopologyType::TRIANGLES),
		ID(0)
	{

	}

	void MeshPrimitive::fromGLTF(
		JSON* meshPrimitiveJSON,
		safety::SafeArray<BufferViewAccessor*>* accessors,
		safety::SafeArray<Material*>* materials)
	{
		JSON* meshAttributesJSON = meshPrimitiveJSON->getFieldObject("attributes");
		collectAttributesFromJSON(meshAttributesJSON, accessors , &attributes);

		JSON* numberJSON = meshPrimitiveJSON->getField("indices");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t indecesAccessorIndex = (int32_t)numberJSON->getNumber();
				indecesAccessor = accessors->getElement(indecesAccessorIndex);
			}
		}

		numberJSON = meshPrimitiveJSON->getField("material");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t materialIndex = (int32_t)numberJSON->getNumber();
				material = materials->getElement(materialIndex);
			}
		}
		else
		{
			material = new Material();
			materials->appendElement(material);
		}
		numberJSON = meshPrimitiveJSON->getField("mode");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				mode = (TopologyType)numberJSON->getNumber();
			}
		}

		JSON* targetsArrayJSON = meshPrimitiveJSON->getFieldArray("targets");
		if (targetsArrayJSON != nullptr)
		{
			size_t targetsArraySize = (size_t)targetsArrayJSON->getArraySize();
			morphTargets.resize(targetsArraySize);

			for (size_t i = 0; i < targetsArraySize; ++i)
			{
				MeshAttributeSet* attributeSet = new MeshAttributeSet();
				morphTargets.setElement(i, attributeSet);
				JSON* targetJSON = targetsArrayJSON->getArrayItem(i);
				collectAttributesFromJSON(targetJSON, accessors, attributeSet);
			}
		}
	}

	BufferViewAccessor* MeshPrimitive::getPositions()
	{
		return attributes.positionAccessor;
	}

	BufferViewAccessor* MeshPrimitive::getNormals()
	{
		return attributes.normalAccessor;
	}

	BufferViewAccessor* MeshPrimitive::getTangents()
	{
		return attributes.tangentAccessor;
	}

	BufferViewAccessor* MeshPrimitive::getTextureCoords_0()
	{
		return attributes.textureCoordAccessor_0;
	}

	BufferViewAccessor* MeshPrimitive::getTextureCoords_1()
	{
		return attributes.textureCoordAccessor_1;
	}

	BufferViewAccessor* MeshPrimitive::getColors_0()
	{
		return attributes.colorAccessor_0;
	}

	BufferViewAccessor* MeshPrimitive::getJoints_0()
	{
		return attributes.jointsAccessor_0;
	}

	BufferViewAccessor* MeshPrimitive::getWeights_0()
	{
		return attributes.weightsAccessor_0;
	}


	BufferViewAccessor* MeshPrimitive::getIndeces()
	{
		return indecesAccessor;
	}

	void MeshPrimitive::collectAttributesFromJSON(JSON* meshAttributesJSON, safety::SafeArray<BufferViewAccessor*>* accessors, MeshAttributeSet* attributes)
	{
		JSON* numberJSON = meshAttributesJSON->getField("POSITION");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t positionAccessorIndex = (int32_t)numberJSON->getNumber();
				attributes->positionAccessor = accessors->getElement(positionAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("NORMAL");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t normalAccessorIndex = (int32_t)numberJSON->getNumber();
				attributes->normalAccessor = accessors->getElement(normalAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("TANGENT");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t tangentAccessorIndex = (int32_t)numberJSON->getNumber();
				attributes->tangentAccessor = accessors->getElement(tangentAccessorIndex);
			}
		}
		numberJSON = meshAttributesJSON->getField("TEXCOORD_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t textureCoordAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				attributes->textureCoordAccessor_0 = accessors->getElement(textureCoordAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("TEXCOORD_1");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t textureCoordAccessorIndex_1 = (int32_t)numberJSON->getNumber();
				attributes->textureCoordAccessor_1 = accessors->getElement(textureCoordAccessorIndex_1);
			}
		}
		numberJSON = meshAttributesJSON->getField("COLOR_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t colorAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				attributes->colorAccessor_0 = accessors->getElement(colorAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("JOINTS_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t jointsAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				attributes->jointsAccessor_0 = accessors->getElement(jointsAccessorIndex_0);
			}
		}
		numberJSON = meshAttributesJSON->getField("WEIGHTS_0");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t weightsAccessorIndex_0 = (int32_t)numberJSON->getNumber();
				attributes->weightsAccessor_0 = accessors->getElement(weightsAccessorIndex_0);
			}
		}
	}
}