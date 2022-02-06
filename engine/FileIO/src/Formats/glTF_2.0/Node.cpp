#include "Node.h"

namespace fileio
{
	Node::Node() :
		skinIndex(-1)
	{

	}

	void Node::fromGLTF(
		JSON* nodeJSON,
		safety::SafeArray<render::Camera*>* cameras,
		safety::SafeArray<mitevox::Mesh*>* meshes,
		safety::SafeArray<Node*>* nodes)
	{
		name = nodeJSON->getFieldString("name");

		JSON* numberJSON = nodeJSON->getField("camera");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t cameraIndex = (int32_t)numberJSON->getNumber();
				camera = cameras->getElement(cameraIndex);
			}
		}
		numberJSON = nodeJSON->getField("skin");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				skinIndex = (int32_t)numberJSON->getNumber();
			}
		}
		numberJSON = nodeJSON->getField("mesh");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t meshIndex = (int32_t)numberJSON->getNumber();
				mesh = meshes->getElement(meshIndex);
			}
		}

		JSON* weightsArrayJSON = nodeJSON->getFieldArray("weights");
		if (weightsArrayJSON != nullptr)
		{
			weightsArrayJSON->toNumberArray<float>(&weights);
		}
		JSON* matrixArrayJSON = nodeJSON->getFieldArray("matrix");
		if (matrixArrayJSON != nullptr)
		{
			matrixArrayJSON->toNumberArray<float>(&matrix);
		}
		JSON* childrenArrayJSON = nodeJSON->getFieldArray("children");
		if (childrenArrayJSON != nullptr)
		{
			size_t childrenCount = childrenArrayJSON->getArraySize();
			children.resize(childrenCount);
			children.fillWithZeros();

			for (size_t i = 0; i < childrenCount; ++i)
			{
				int32_t childIndex = (int32_t)childrenArrayJSON->getArrayItemNumber(i);
				children.setElement(i, nodes->getElement(childIndex));
			}
		}
	}
}