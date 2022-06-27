#include "applyMorphTargetAnimation.h"

namespace mitevox
{
	void applyMorphTargetAnimation(Node* meshNode)
	{
		size_t primitivesCount = meshNode->mesh->primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* meshPrimitive = meshNode->mesh->primitives.getElement(primitiveIndex);
			MeshPrimitive* meshAnimationTargetPrimitive = meshNode->meshAnimationTarget->primitives.getElement(primitiveIndex);
			size_t vertecesCount = meshPrimitive->getVertecesCount();
			for (size_t vertexIndex = 0; vertexIndex < vertecesCount; ++vertexIndex)
			{
				mathem::Vector3D resultPosition = meshPrimitive->getVertexPosition(vertexIndex);

				size_t morphTargetsCount = meshNode->mesh->getMorphTargetsCount();
				for (size_t morphTargetIndex = 0; morphTargetIndex < morphTargetsCount; ++morphTargetIndex)
				{
					float weight = meshNode->mesh->weights.getElement(morphTargetIndex);
					resultPosition += meshPrimitive->getMorphVertexPosition(morphTargetIndex, vertexIndex) * weight;
				}
				meshAnimationTargetPrimitive->setVertexPosition(vertexIndex, resultPosition);
			}
		}
	}
}