#ifndef SKELETONBASE_H
#define SKELETONBASE_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

#include <string>

namespace mitevox
{
	class SkeletonBase
	{
	public:

		safety::SafeArray<mathem::GeometryTransform> inverseBindTransforms; // They define "rest" pose of the skeleton
		safety::SafeArray<mathem::Matrix4x4> jointMatrices;
		std::string name;
		mathem::GeometryTransform globalTransform;

		SkeletonBase();
		SkeletonBase(size_t jointsCount);
		void init(size_t jointsCount);
		size_t getJointsCount();

		virtual void updateJointMatrices() = 0;
	};
}

#endif

