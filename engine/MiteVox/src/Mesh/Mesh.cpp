#include "Mesh.h"

namespace mitevox
{
	Mesh::Mesh()
	{
		type = mathem::GeometryPrimitiveType::MESH;
	}

	void Mesh::makeCopyForAnimationTo(Mesh* resultMesh)
	{
		size_t primitivesCount = primitives.getElementsCount();
		resultMesh->primitives.resize(primitivesCount);
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = new MeshPrimitive();
			primitives.getElement(primitiveIndex)->makeCopyForAnimationTo(primitive);
			resultMesh->primitives.setElement(primitiveIndex, primitive);
		}
		resultMesh->name = name;

		/// NOTE: We don't copy weights because resultMesh will store only the result of animation.
	}

	bool Mesh::isTriangularMesh()
	{
		// TODO:

		return true;
	}

	size_t Mesh::getVertecesCount()
	{
		size_t resultPointsCount = 0;
		
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			resultPointsCount += primitive->getVertecesCount();
		}

		return resultPointsCount;
	}

	mathem::Vector3D Mesh::getVertexPosition(uint32_t index)
	{
		mathem::Vector3D resultPoint;
		resultPoint.setAll(0.0f);

		uint32_t primitivesCount = primitives.getElementsCount();
		for (uint32_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);

			// TODO:
			/*if ()
			{

			}*/
		}

		return resultPoint;
	}

	size_t Mesh::getTrianglesCount()
	{
		size_t resultTrianglesCount = 0;

		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
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

	void setVertexPosition(uint32_t index, mathem::Vector3D value)
	{
		// TODO:
	}

	bool Mesh::isMorphable()
	{
		return getMorphTargetsCount() > 0;
	}

	size_t Mesh::getMorphTargetsCount()
	{
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			size_t primitiveMorphTargetsCount = primitive->morphTargets.getElementsCount();
			if (primitiveMorphTargetsCount > 0)
			{
				return primitiveMorphTargetsCount;
			}
		}
		return 0;
	}
}