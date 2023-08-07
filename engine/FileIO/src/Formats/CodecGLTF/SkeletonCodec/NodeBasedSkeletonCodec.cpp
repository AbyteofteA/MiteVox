#include "NodeBasedSkeletonCodec.h"

#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Convertations.h"

namespace fileio
{
    void NodeBasedSkeletonCodec::fromGLTF(
        mitevox::NodeBasedSkeleton* skeletonResult,
        JSON* skeletonJSON,
		safety::SafeArray<mitevox::Node*>* nodes,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors)
    {
		skeletonResult->name = skeletonJSON->getFieldStringOrDefault("name", "Untitled");

		int32_t skeletonRootNodeIndex = (int32_t)skeletonJSON->getFieldNumberOrDefault("skeleton", -1.0f);
		if (skeletonRootNodeIndex >= 0)
		{
			skeletonResult->root = nodes->getElement(skeletonRootNodeIndex);
		}
		
		size_t jointsCount = 0;
		if (JSON* jointsArrayJSON = skeletonJSON->getField("joints"))
		{
			jointsCount = jointsArrayJSON->getArraySize();
			skeletonResult->init(jointsCount);

			skeletonResult->joints.resize(jointsCount);
			skeletonResult->joints.setAllElementsZeros();

			for (size_t i = 0; i < jointsCount; ++i)
			{
				int32_t jointNodeIndex = (int32_t)jointsArrayJSON->getArrayItemNumberOrDefault(i, -1.0f);
				skeletonResult->joints.setElement(i, nodes->getElement(jointNodeIndex));
			}
		}

		if (JSON* numberJSON = skeletonJSON->getField("inverseBindMatrices"))
		{
			int32_t inverseBindMatricesAccessorIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
			mitevox::BufferViewAccessor* inverseBindMatricesAccessor = accessors->getElement(inverseBindMatricesAccessorIndex);

			for (size_t i = 0; i < jointsCount; ++i)
			{
				mathem::Matrix4x4 inverseBindMatrix = inverseBindMatricesAccessor->getMatrix4x4(i);
				mathem::GeometryTransform inverseBindTransform = mathem::matrixToTransform(inverseBindMatrix);
				skeletonResult->inverseBindTransforms.setElement(i, inverseBindTransform);
			}
		}
    }

	void NodeBasedSkeletonCodec::toGLTF(
		JSON* skeletonJSONResult,
		mitevox::NodeBasedSkeleton* skeleton,
		safety::SafeArray<mitevox::Node*>* nodes,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors)
	{
		skeletonJSONResult->setField("name", skeleton->name);

		if (skeleton->root)
		{
			size_t nodesCount = nodes->getElementsCount();
			for (size_t i = 0; i < nodesCount; ++i)
			{
				if (skeleton->root == nodes->getElement(i))
				{
					skeletonJSONResult->setField("skeleton", (double)i);
					break;
				}
			}
		}
		
		size_t nodesCount = nodes->getElementsCount();
		size_t jointsCount = skeleton->getJointsCount();
		if (jointsCount > 0)
		{
			JSON* jointsArrayJSON = skeletonJSONResult->setFieldType("joints", JSONtype::ARRAY);
			for (size_t jointIndex = 0; jointIndex < jointsCount; ++jointIndex)
			{
				for (size_t nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex)
				{
					if (skeleton->joints.getElement(jointIndex) == nodes->getElement(nodeIndex))
					{
						jointsArrayJSON->addArrayItem((double)nodeIndex);
					}
				}
			}

			/*size_t accessorsCount = accessors->getElementsCount();
			for (size_t i = 0; i < accessorsCount; ++i)
			{
				if (skeleton->inverseBindTransforms == accessors->getElement(i))
				{
					skeletonJSONResult->setField("inverseBindMatrices", (double)i);
				}
			}*/
		}
	}
}