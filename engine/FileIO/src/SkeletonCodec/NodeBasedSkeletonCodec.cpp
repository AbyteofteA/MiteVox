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
		skeletonResult->name = skeletonJSON->getFieldString("name");

		JSON* jointsArrayJSON = skeletonJSON->getField("joints");
		size_t jointsCount = 0;
		if (jointsArrayJSON != nullptr)
		{
			jointsCount = jointsArrayJSON->getArraySize();
			skeletonResult->init(jointsCount);

			skeletonResult->joints.resize(jointsCount);
			skeletonResult->joints.fillWithZeros();

			for (size_t i = 0; i < jointsCount; ++i)
			{
				int32_t jointNodeIndex = (int32_t)jointsArrayJSON->getArrayItemNumber(i);
				skeletonResult->joints.setElement(i, nodes->getElement(jointNodeIndex));
			}
		}

		JSON* numberJSON = skeletonJSON->getField("inverseBindMatrices");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t inverseBindMatricesAccessorIndex = (int32_t)numberJSON->getNumber();
				mitevox::BufferViewAccessor* inverseBindMatricesAccessor = accessors->getElement(inverseBindMatricesAccessorIndex);

				for (size_t i = 0; i < jointsCount; ++i)
				{
					mathem::GeometryTransform inverseBindTransform = mathem::matrixToTransform(inverseBindMatricesAccessor->getMatrix4x4(i));
					skeletonResult->inverseBindTransforms.setElement(i, inverseBindTransform);
				}
			}
		}

    }
}