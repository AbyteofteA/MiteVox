#include "Node.h"

#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"

namespace mitevox
{
	bool Node::isMorphableMesh()
	{
		if (mesh != nullptr)
		{
			if (mesh->isMorphable())
			{
				return true;
			}
		}
		return false;
	}

	Mesh* Node::getMeshToRender()
	{
		if (isMorphableMesh())
		{
			return morphAnimationTarget;
		}
		return mesh;
	}

	void Node::tryGenerateHitbox()
	{
		if (mesh == nullptr)
		{
			// TODO: create collider based on the transform
			return;
		}

		if (collider == nullptr)
		{
			collider = new mathem::ComplexGeometry();
		}

		if (collider->primitives.getElementsCount() != 0)
		{
			return;
		}

		// TODO: calculate min and max recursively
		mathem::Vector3D min = mesh->getMinPosition();
		mathem::Vector3D max = mesh->getMaxPosition();

		mathem::BoxGeometry* boundingBox = new mathem::BoxGeometry();
		boundingBox->halfSize = (max - min) * 0.5f;
		boundingBox->transform.translation = (max + min) * 0.5f;

		collider->primitives.appendElement(boundingBox);
	}

	mathem::GeometryTransform* Node::getTransform()
	{
		return &transform;
	}

	mathem::ComplexGeometry* Node::getCollider()
	{
		return collider;
	}
}