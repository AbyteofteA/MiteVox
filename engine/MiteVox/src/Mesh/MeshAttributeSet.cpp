#include "MeshAttributeSet.h"

namespace mitevox
{
	void MeshAttributeSet::makeSeparateCopyTo(MeshAttributeSet* resultMeshAttributeSet)
	{
		if (positionAccessor)
		{
			resultMeshAttributeSet->positionAccessor = new BufferViewAccessor();
			positionAccessor->makeSeparateCopyTo(resultMeshAttributeSet->positionAccessor);
		}
		if (normalAccessor)
		{
			resultMeshAttributeSet->normalAccessor = new BufferViewAccessor();
			normalAccessor->makeSeparateCopyTo(resultMeshAttributeSet->normalAccessor);
		}
		if (tangentAccessor)
		{
			resultMeshAttributeSet->tangentAccessor = new BufferViewAccessor();
			tangentAccessor->makeSeparateCopyTo(resultMeshAttributeSet->tangentAccessor);
		}
		if (textureCoordAccessor_0)
		{
			resultMeshAttributeSet->textureCoordAccessor_0 = new BufferViewAccessor();
			textureCoordAccessor_0->makeSeparateCopyTo(resultMeshAttributeSet->textureCoordAccessor_0);
		}
		if (textureCoordAccessor_1)
		{
			resultMeshAttributeSet->textureCoordAccessor_1 = new BufferViewAccessor();
			textureCoordAccessor_1->makeSeparateCopyTo(resultMeshAttributeSet->textureCoordAccessor_1);
		}
		if (colorAccessor_0)
		{
			resultMeshAttributeSet->colorAccessor_0 = new BufferViewAccessor();
			colorAccessor_0->makeSeparateCopyTo(resultMeshAttributeSet->colorAccessor_0);
		}
		if (jointsAccessor_0)
		{
			resultMeshAttributeSet->jointsAccessor_0 = new BufferViewAccessor();
			jointsAccessor_0->makeSeparateCopyTo(resultMeshAttributeSet->jointsAccessor_0);
		}
		if (weightsAccessor_0)
		{
			resultMeshAttributeSet->weightsAccessor_0 = new BufferViewAccessor();
			weightsAccessor_0->makeSeparateCopyTo(resultMeshAttributeSet->weightsAccessor_0);
		}
	}

}