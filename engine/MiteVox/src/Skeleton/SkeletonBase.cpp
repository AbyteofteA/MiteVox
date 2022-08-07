#include "SkeletonBase.h"

namespace mitevox
{
	SkeletonBase::SkeletonBase()
	{

	}

	SkeletonBase::SkeletonBase(size_t jointsCount)
	{
		init(jointsCount);
	}

	void SkeletonBase::init(size_t jointsCount)
	{
		inverseBindTransforms.resize(jointsCount);
		jointMatrices.resize(jointsCount);

		mathem::Matrix4x4 identityMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
		jointMatrices.setAllElements(identityMatrix);
	}

	size_t SkeletonBase::getJointsCount()
	{
		return jointMatrices.getElementsCount();
	}
}