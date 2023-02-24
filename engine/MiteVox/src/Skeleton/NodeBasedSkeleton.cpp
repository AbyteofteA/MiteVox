#include "NodeBasedSkeleton.h"

#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/Math/src/Convertations.h"

namespace mitevox
{
	NodeBasedSkeleton::NodeBasedSkeleton() : SkeletonBase()
	{

	}

	NodeBasedSkeleton::NodeBasedSkeleton(size_t jointsCount) : SkeletonBase(jointsCount)
	{

	}

	void NodeBasedSkeleton::updateJointMatrices()
	{
		for (size_t i = 0; i < joints.getElementsCount(); ++i)
		{
			size_t nodesProcessed = applySkeletalAnimationRecursiveHelper(
				joints.getElement(i),
				mathem::GeometryTransform());
			i += nodesProcessed - 1;
		}
	}

	int32_t NodeBasedSkeleton::getJointIndex(Node* node)
	{
		for (size_t i = 0; i < joints.getElementsCount(); ++i)
		{
			if (joints.getElement(i) == node)
			{
				return i;
			}
		}
		return -1;
	}

	size_t NodeBasedSkeleton::applySkeletalAnimationRecursiveHelper(
		Node* jointNode,
		mathem::GeometryTransform parentTransform)
	{
		int32_t actualJointIndex = getJointIndex(jointNode);

		parentTransform *= jointNode->transform;

		size_t nodesProcessed = 0;
		if (actualJointIndex >= 0)
		{
			mathem::GeometryTransform inverseBindTransform = inverseBindTransforms.getElement(actualJointIndex);
			mathem::GeometryTransform jointTransform = parentTransform * inverseBindTransform;
			jointMatrices.setElement(actualJointIndex, mathem::transformToMatrix(jointTransform));
			nodesProcessed += 1;
		}

		for (size_t i = 0; i < jointNode->children.getElementsCount(); ++i)
		{
			nodesProcessed += applySkeletalAnimationRecursiveHelper(jointNode->children.getElement(i), parentTransform);
		}

		return nodesProcessed;
	}
}