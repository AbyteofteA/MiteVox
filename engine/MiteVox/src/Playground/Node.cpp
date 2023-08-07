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

	Mesh* Node::tryAttachNewMesh()
	{
		if (mesh == nullptr)
		{
			mesh = new Mesh();
		}
		return mesh;
	}

	bool Node::hasCameraRecursively()
	{
		if (camera)
		{
			return true;
		}
		else
		{
			size_t childrenNodesCount = children.getElementsCount();
			for (size_t i = 0; i < childrenNodesCount; ++i)
			{
				Node* node = children.getElement(i);
				bool result = node->hasCameraRecursively();
				if (result)
				{
					return true;
				}
			}
		}
		return false;
	}

	mathem::GeometryTransform Node::getCameraRecursively(render::Camera** cameraResult, mathem::GeometryTransform* parentTransform)
	{
		mathem::GeometryTransform rootTransform = *parentTransform * transform;

		if (camera)
		{
			*cameraResult = camera;
			return rootTransform;
		}
		else
		{
			*cameraResult = nullptr;
			size_t childrenNodesCount = children.getElementsCount();
			for (size_t i = 0; i < childrenNodesCount; ++i)
			{
				Node* node = children.getElement(i);
				mathem::GeometryTransform result = node->getCameraRecursively(cameraResult, &rootTransform);
				if (*cameraResult)
				{
					return result;
				}
			}
		}
		return rootTransform;
	}

	void Node::getMinMaxRecursively(mathem::GeometryTransform* parentTransform, mathem::Vector3D* min, mathem::Vector3D* max)
	{
		mathem::GeometryTransform rootTransform = *parentTransform * transform;

		if (skeleton)
		{
			// TODO: 
		}

		if (mesh)
		{
			size_t vertecesCount = mesh->getVertecesCount();
			for (size_t i = 0; i < vertecesCount; ++i)
			{
				mathem::Vector3D vertexPosition = mesh->getVertexPosition(i);
				rootTransform.applyTo(vertexPosition);
				*min = mathem::Vector3D::min(min, &vertexPosition);
				*max = mathem::Vector3D::max(max, &vertexPosition);
			}
		}

		if (morphAnimationTarget)
		{
			// TODO: 
		}

		size_t childrenNodesCount = children.getElementsCount();
		for (size_t i = 0; i < childrenNodesCount; ++i)
		{
			Node* node = children.getElement(i);
			node->getMinMaxRecursively(&rootTransform, min, max);
		}
	}
}