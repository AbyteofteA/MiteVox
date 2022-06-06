#include "Mesh.h"

namespace mitevox
{
	Mesh::Mesh()
	{
		type = mathem::GeometryPrimitiveType::MESH;
	}

	bool Mesh::isTriangularMesh()
	{
		// TODO:

		return true;
	}

	uint32_t Mesh::getVertecesCount()
	{
		uint32_t resultPointsCount = 0;
		
		uint32_t primitivesCount = primitives.getElementsCount();
		for (uint32_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			resultPointsCount += primitive->getVertecesCount();
		}

		return resultPointsCount;
	}

	mathem::Vector3D Mesh::getVertexPosition(uint32_t index)
	{
		mathem::Vector3D resultPoint;

		uint32_t primitivesCount = primitives.getElementsCount();
		for (uint32_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* primitive = primitives.getElement(primitiveIndex);

			// TODO:
			/*if ()
			{

			}*/
		}

		return resultPoint;
	}

	uint32_t Mesh::getTrianglesCount()
	{
		uint32_t resultTrianglesCount = 0;

		uint32_t primitivesCount = primitives.getElementsCount();
		for (uint32_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			resultTrianglesCount += primitive->getTrianglesCount();
		}

		return resultTrianglesCount;
	}

	mathem::TriangleGeometry Mesh::getTrianglePositions(uint32_t index)
	{
		// TODO:

		return mathem::TriangleGeometry();
	}

	bool Mesh::isIdealGeometry()
	{
		return false;
	}
}