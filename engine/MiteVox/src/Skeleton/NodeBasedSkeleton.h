#ifndef NODEBASEDSKELETON_H
#define NODEBASEDSKELETON_H

#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace mitevox
{
	class Node;

	class NodeBasedSkeleton : public SkeletonBase
	{
	public:

		Node* root = nullptr;
		safety::SafeArray<Node*> joints;

		NodeBasedSkeleton();
		NodeBasedSkeleton(size_t jointsCount);

		void updateJointMatrices();

	private:

		int32_t getJointIndex(Node* node);
		size_t applySkeletalAnimationRecursiveHelper(
			Node* jointNode,
			mathem::GeometryTransform parentTransform);
	};
}

#endif