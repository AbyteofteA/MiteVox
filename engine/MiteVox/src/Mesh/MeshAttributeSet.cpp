#include "MeshAttributeSet.h"

namespace mitevox
{
	MeshAttributeSet::MeshAttributeSet()
	{
		byName.positionAccessor = nullptr;
		byName.normalAccessor = nullptr;
		byName.textureCoordAccessor_0 = nullptr;
		byName.textureCoordAccessor_1 = nullptr;
		byName.colorAccessor_0 = nullptr;
		byName.jointsAccessor_0 = nullptr;
		byName.weightsAccessor_0 = nullptr;
		byName.tangentAccessor = nullptr;
	}

	void MeshAttributeSet::makeSeparateCopyTo(MeshAttributeSet* resultMeshAttributeSet)
	{
		for (size_t i = 0; i < attributesCount; ++i)
		{
			if (this->byIndex[i] != nullptr)
			{
				resultMeshAttributeSet->byIndex[i] = new BufferViewAccessor();
				this->byIndex[i]->makeSeparateCopyTo(resultMeshAttributeSet->byIndex[i]);
			}
		}
	}

}