#include "MeshAttributeSet.h"

namespace mitevox
{
	MeshAttributeSet::MeshAttributeSet()
	{

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