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

		size_t jointsCount = 0;
		if (JSON* jointsArrayJSON = skeletonJSON->getField("joints"))
		{
			jointsCount = jointsArrayJSON->getArraySize();
			skeletonResult->init(jointsCount);

			skeletonResult->joints.resize(jointsCount);
			skeletonResult->joints.fillWithZeros();

			for (size_t i = 0; i < jointsCount; ++i)
			{
				int32_t jointNodeIndex = (int32_t)jointsArrayJSON->getArrayItemNumberOrDefault(i, -1.0);
				skeletonResult->joints.setElement(i, nodes->getElement(jointNodeIndex));
			}
		}

		if (JSON* numberJSON = skeletonJSON->getField("inverseBindMatrices"))
		{
			int32_t inverseBindMatricesAccessorIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			mitevox::BufferViewAccessor* inverseBindMatricesAccessor = accessors->getElement(inverseBindMatricesAccessorIndex);

			for (size_t i = 0; i < jointsCount; ++i)
			{
				mathem::Matrix4x4 inverseBindMatrix = inverseBindMatricesAccessor->getMatrix4x4(i);
				mathem::GeometryTransform inverseBindTransform = mathem::matrixToTransform(inverseBindMatrix);
				skeletonResult->inverseBindTransforms.setElement(i, inverseBindTransform);
			}
		}
    }
}