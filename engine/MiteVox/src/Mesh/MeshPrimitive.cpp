#include "MeshPrimitive.h"

namespace mitevox
{
	mitevox::BufferViewAccessor* MeshPrimitive::getPositions()
	{
		return attributes.positionAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getNormals()
	{
		return attributes.normalAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTangents()
	{
		return attributes.tangentAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_0()
	{
		return attributes.textureCoordAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_1()
	{
		return attributes.textureCoordAccessor_1;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getColors_0()
	{
		return attributes.colorAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getJoints_0()
	{
		return attributes.jointsAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getWeights_0()
	{
		return attributes.weightsAccessor_0;
	}


	mitevox::BufferViewAccessor* MeshPrimitive::getIndeces()
	{
		return indecesAccessor;
	}

	bool MeshPrimitive::isTriangularMesh()
	{
		switch (mode)
		{
		case mitevox::TRIANGLES:
		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return true;

		default:
			return false;
		}
	}

	uint32_t MeshPrimitive::getPointsCount()
	{
		return getPositions()->count;
	}

	mathem::Vector3D MeshPrimitive::getPoint(uint32_t index)
	{
		mathem::Vector3D resultPoint;

		// TODO:

		return resultPoint;
	}

	uint32_t MeshPrimitive::getTrianglesCount()
	{
		// TODO:

		return 0;
	}

	mathem::TriangleGeometry MeshPrimitive::getTriangle(uint32_t index)
	{
		// TODO:

		return mathem::TriangleGeometry();
	}

}