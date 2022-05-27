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

	uint32_t Mesh::getPointsCount()
	{
		uint32_t resultPointsCount = 0;
		
		uint32_t primitivesCount = primitives.getElementsCount();
		for (uint32_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			resultPointsCount += primitive->getPointsCount();
		}

		return resultPointsCount;
	}

	mathem::Vector3D Mesh::getPoint(uint32_t index)
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

	mathem::Vector3D Mesh::getPoint(uint32_t index, mathem::GeometryTransform* globalTransform)
	{
		// TODO:

		return mathem::Vector3D();
	}

	uint32_t Mesh::getTrianglesCount()
	{
		// TODO:

		return 0;
	}

	mathem::TriangleGeometry Mesh::getTriangle(uint32_t index)
	{
		// TODO:

		return mathem::TriangleGeometry();
	}

	mathem::TriangleGeometry Mesh::getTriangle(uint32_t index, mathem::GeometryTransform* globalTransform)
	{
		// TODO:

		return mathem::TriangleGeometry();
	}

	bool Mesh::isIdealGeometry()
	{
		return false;
	}
}