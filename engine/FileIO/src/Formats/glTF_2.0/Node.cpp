#include "Node.h"

namespace fileio
{
	Node::Node() :
		cameraIndex(-1),
		skinIndex(-1),
		meshIndex(-1)
	{

	}

	void Node::fromGLTF(JSON* nodeJSON)
	{
		name = nodeJSON->getFieldString("name");

		JSON* numberJSON = nodeJSON->getField("camera");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				cameraIndex = (int32_t)numberJSON->getNumber();
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
				meshIndex = (int32_t)numberJSON->getNumber();
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
			childrenArrayJSON->toNumberArray<int32_t>(&children);
		}
	}
}