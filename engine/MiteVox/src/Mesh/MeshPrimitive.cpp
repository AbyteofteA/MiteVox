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

	mathem::TriangleGeometry MeshPrimitive::getTrianglePositions(uint32_t index)
	{
		mathem::TriangleGeometry resultTriangle;

		switch (topologyType)
		{
		case mitevox::TRIANGLES:
		{
			uint32_t firstPointIndex = index * 3;
			resultTriangle.point1 = getVertexPosition(firstPointIndex);
			resultTriangle.point2 = getVertexPosition(firstPointIndex + 1);
			resultTriangle.point3 = getVertexPosition(firstPointIndex + 2);
			break;
		}
		case mitevox::TRIANGLE_STRIP:
		{
			resultTriangle.point1 = getVertexPosition(index);
			if (index & 1)
			{
				resultTriangle.point2 = getVertexPosition(index + 2);
				resultTriangle.point3 = getVertexPosition(index + 1);
			}
			else
			{
				resultTriangle.point2 = getVertexPosition(index + 1);
				resultTriangle.point3 = getVertexPosition(index + 2);
			}
			break;
		}
		case mitevox::TRIANGLE_FAN:
		{
			uint32_t secondPointIndex = index * 2 + 1;
			resultTriangle.point1 = getVertexPosition(secondPointIndex);
			resultTriangle.point2 = getVertexPosition(secondPointIndex + 1);
			resultTriangle.point3 = getVertexPosition(0);
			break;
		}
		default:
			break;
		}

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