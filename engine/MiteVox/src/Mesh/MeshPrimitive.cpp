#include "MeshPrimitive.h"

namespace mitevox
{
	void MeshPrimitive::makeCopyForAnimationTo(MeshPrimitive* resultMeshPrimitive)
	{
		attributes.makeSeparateCopyTo(&resultMeshPrimitive->attributes);
		if (indecesAccessor)
		{
			resultMeshPrimitive->indecesAccessor = new BufferViewAccessor();
			indecesAccessor->makeSeparateCopyTo(resultMeshPrimitive->indecesAccessor);
		}
		resultMeshPrimitive->material = material; // TODO: copy material
		resultMeshPrimitive->topologyType = topologyType;
		resultMeshPrimitive->ID = 0;

		/// NOTE: We don't copy morphTargets because resultMeshPrimitive will store 
		/// only the result of animation.
	}

	void MeshPrimitive::tryGenerateTangents()
	{
		return;

		if (attributes.byName.normalAccessor == nullptr ||
			attributes.byName.tangentAccessor != nullptr ||
			topologyType != TopologyType::TRIANGLES)
		{
			return;
		}

		attributes.byName.tangentAccessor = new BufferViewAccessor();
		attributes.byName.tangentAccessor->count = attributes.byName.normalAccessor->count;
		attributes.byName.tangentAccessor->componentType = attributes.byName.normalAccessor->componentType;
		attributes.byName.tangentAccessor->type = BufferViewAccessor::Type::VEC4;
		
		// TODO: tryGenerateTangents()
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getPositions()
	{
		return attributes.byName.positionAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getNormals()
	{
		return attributes.byName.normalAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTangents()
	{
		return attributes.byName.tangentAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_0()
	{
		return attributes.byName.textureCoordAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_1()
	{
		return attributes.byName.textureCoordAccessor_1;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getColors_0()
	{
		return attributes.byName.colorAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getJoints_0()
	{
		return attributes.byName.jointsAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getWeights_0()
	{
		return attributes.byName.weightsAccessor_0;
	}


	mitevox::BufferViewAccessor* MeshPrimitive::getIndeces()
	{
		return indecesAccessor;
	}

	mathem::Vector3D MeshPrimitive::getMinPosition()
	{
		safety::SafeFloatArray minArray = getPositions()->min;
		mathem::Vector3D min(&minArray);
		return min;
	}

	mathem::Vector3D MeshPrimitive::getMaxPosition()
	{
		safety::SafeFloatArray maxArray = getPositions()->max;
		mathem::Vector3D max(&maxArray);
		return max;
	}

	bool MeshPrimitive::isTriangularMesh()
	{
		switch (topologyType)
		{
		case mitevox::TRIANGLES:
		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return true;

		default:
			return false;
		}
	}

	uint32_t MeshPrimitive::getVertecesCount()
	{
		if (indecesAccessor != nullptr)
		{
			return getIndeces()->count;
		}
		else
		{
			return getPositions()->count;
		}
	}

	mathem::Vector3D MeshPrimitive::getVertexPosition(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getPositions()->getVector3D(actualPointIndex);
		}
		else
		{
			return getPositions()->getVector3D(index);
		}
	}

	mathem::Vector2D MeshPrimitive::getVertexTextureCoords_0(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getTextureCoords_0()->getVector2D(actualPointIndex);
		}
		else
		{
			return getTextureCoords_0()->getVector2D(index);
		}
	}

	mathem::Vector2D MeshPrimitive::getVertexTextureCoords_1(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getTextureCoords_1()->getVector2D(actualPointIndex);
		}
		else
		{
			return getTextureCoords_1()->getVector2D(index);
		}
	}

	uint32_t MeshPrimitive::getTrianglesCount()
	{
		uint32_t pointsCount = getVertecesCount();

		switch (topologyType)
		{
		case mitevox::TRIANGLES:
			return pointsCount / 3;

		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return pointsCount - 2;

		default:
			return 0;
		}

		return 0;
	}

	void MeshPrimitive::getTriangleVertexIndeces(uint32_t triangleIndex, uint32_t* vertex1, uint32_t* vertex2, uint32_t* vertex3)
	{
		switch (topologyType)
		{
		case mitevox::TRIANGLES:
		{
			uint32_t firstPointIndex = triangleIndex * 3;
			*vertex1 = firstPointIndex;
			*vertex2 = firstPointIndex + 1;
			*vertex3 = firstPointIndex + 2;
			break;
		}
		case mitevox::TRIANGLE_STRIP:
		{
			*vertex1 = triangleIndex;
			if (triangleIndex & 1)
			{
				*vertex2 = triangleIndex + 2;
				*vertex3 = triangleIndex + 1;
			}
			else
			{
				*vertex2 = triangleIndex + 1;
				*vertex3 = triangleIndex + 2;
			}
			break;
		}
		case mitevox::TRIANGLE_FAN:
		{
			uint32_t secondPointIndex = triangleIndex * 2 + 1;
			*vertex1 = secondPointIndex;
			*vertex2 = secondPointIndex + 1;
			*vertex3 = 0;
			break;
		}
		default:
			break;
		}
	}

	mathem::TriangleGeometry3D MeshPrimitive::getTrianglePositions(uint32_t index)
	{
		mathem::TriangleGeometry3D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexPosition(vertex1);
		resultTriangle.point2 = getVertexPosition(vertex2);
		resultTriangle.point3 = getVertexPosition(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry2D MeshPrimitive::getTriangleTextureCoords_0(uint32_t index)
	{
		mathem::TriangleGeometry2D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexTextureCoords_0(vertex1);
		resultTriangle.point2 = getVertexTextureCoords_0(vertex2);
		resultTriangle.point3 = getVertexTextureCoords_0(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry2D MeshPrimitive::getTriangleTextureCoords_1(uint32_t index)
	{
		mathem::TriangleGeometry2D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexTextureCoords_1(vertex1);
		resultTriangle.point2 = getVertexTextureCoords_1(vertex2);
		resultTriangle.point3 = getVertexTextureCoords_1(vertex3);
		return resultTriangle;
	}

	void MeshPrimitive::setVertexPosition(uint32_t index, mathem::Vector3D position)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			getPositions()->setVector3D(actualPointIndex, position);
		}
		else
		{
			getPositions()->setVector3D(index, position);
		}
	}

	mathem::Vector3D MeshPrimitive::getMorphVertexPosition(uint32_t morphIndex, uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return morphTargets.getElement(morphIndex)->byName.positionAccessor->getVector3D(actualPointIndex);
		}
		else
		{
			return morphTargets.getElement(morphIndex)->byName.positionAccessor->getVector3D(index);
		}
	}

	void MeshPrimitive::setMorphVertexPosition(uint32_t morphIndex, uint32_t index, mathem::Vector3D position)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			morphTargets.getElement(morphIndex)->byName.positionAccessor->setVector3D(actualPointIndex, position);
		}
		else
		{
			morphTargets.getElement(morphIndex)->byName.positionAccessor->setVector3D(index, position);
		}
	}
}