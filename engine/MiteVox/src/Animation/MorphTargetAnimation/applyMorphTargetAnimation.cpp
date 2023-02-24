#include "applyMorphTargetAnimation.h"

namespace mitevox
{
	void applyMorphTargetAnimation(Node* meshNode)
	{
		size_t morphTargetsCount = meshNode->mesh->getMorphTargetsCount();
		if (morphTargetsCount == 0)
		{
			return;
		}

		size_t primitivesCount = meshNode->mesh->primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* meshPrimitive = meshNode->mesh->primitives.getElement(primitiveIndex);
			MeshPrimitive* meshAnimationTargetPrimitive = meshNode->morphAnimationTarget->primitives.getElement(primitiveIndex);
			size_t vertecesCount = meshPrimitive->getVertecesCount();
			for (size_t vertexIndex = 0; vertexIndex < vertecesCount; ++vertexIndex)
			{
				// Morph positions
				mathem::Vector3D resultPosition = meshPrimitive->getVertexPosition(vertexIndex);
				for (size_t morphTargetIndex = 0; morphTargetIndex < morphTargetsCount; ++morphTargetIndex)
				{
					float weight = meshNode->mesh->weights.getElement(morphTargetIndex);
					resultPosition += meshPrimitive->getMorphVertexPosition(morphTargetIndex, vertexIndex) * weight;
				}
				meshAnimationTargetPrimitive->setVertexPosition(vertexIndex, resultPosition);

				// Morph normals if any
				MeshAttributeSet* firstMorphAttributeSet = meshPrimitive->morphTargets.getElement(0);
				if (firstMorphAttributeSet->byName.normalAccessor)
				{
					mathem::Vector3D resultNormal = meshPrimitive->getVertexNormal(vertexIndex);
					for (size_t morphTargetIndex = 0; morphTargetIndex < morphTargetsCount; ++morphTargetIndex)
					{
						float weight = meshNode->mesh->weights.getElement(morphTargetIndex);
						resultNormal += meshPrimitive->getMorphVertexNormal(morphTargetIndex, vertexIndex) * weight;
					}
					meshAnimationTargetPrimitive->setVertexNormal(vertexIndex, resultNormal);
				}
			}
		}
	}
}