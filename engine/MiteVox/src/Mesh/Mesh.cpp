#include "Mesh.h"

namespace mitevox
{
	Mesh::Mesh()
	{
		type = mathem::GeometryPrimitiveType::MESH;
	}

	IlluminationModel Mesh::getIlluminationModel()
	{
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			return primitive->material->illuminationModel;
		}
		return IlluminationModel::NONE;
	}

	void Mesh::setIlluminationModel(IlluminationModel illuminationModel)
	{
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			primitive->material->illuminationModel = illuminationModel;
		}
	}

	MeshPrimitive* Mesh::addMeshPrimitive()
	{
		MeshPrimitive* meshPrimitive = new MeshPrimitive();
		primitives.appendElement(meshPrimitive);
		return meshPrimitive;
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

	void Mesh::tryGenerateTangents()
	{
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			primitive->tryGenerateTangents();
		}
	}

	mathem::Vector3D Mesh::getMinPosition()
	{
		mathem::Vector3D resultMinPosition = primitives.getElement(0)->getMinPosition();
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 1; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			mathem::Vector3D tmpMinPosition = primitives.getElement(primitiveIndex)->getMinPosition();
			resultMinPosition.x() = std::min(resultMinPosition.x(), tmpMinPosition.x());
			resultMinPosition.y() = std::min(resultMinPosition.y(), tmpMinPosition.y());
			resultMinPosition.z() = std::min(resultMinPosition.z(), tmpMinPosition.z());
		}
		return resultMinPosition;
	}

	mathem::Vector3D Mesh::getMaxPosition()
	{
		mathem::Vector3D resultMaxPosition = primitives.getElement(0)->getMaxPosition();
		size_t primitivesCount = primitives.getElementsCount();
		for (size_t primitiveIndex = 1; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			mathem::Vector3D tmpMaxPosition = primitives.getElement(primitiveIndex)->getMaxPosition();
			resultMaxPosition.x() = std::max(resultMaxPosition.x(), tmpMaxPosition.x());
			resultMaxPosition.y() = std::max(resultMaxPosition.y(), tmpMaxPosition.y());
			resultMaxPosition.z() = std::max(resultMaxPosition.z(), tmpMaxPosition.z());
		}
		return resultMaxPosition;
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
		uint32_t primitivesCount = primitives.getElementsCount();
		for (uint32_t primitiveIndex = 0; primitiveIndex < primitivesCount; ++primitiveIndex)
		{
			MeshPrimitive* primitive = primitives.getElement(primitiveIndex);
			uint32_t primitiveVertecesCount = primitive->getVertecesCount();
			
			if (index >= primitiveVertecesCount)
			{
				index -= primitiveVertecesCount;
			}
			else
			{
				return primitive->getVertexPosition(index);
			}
		}
		return mathem::Vector3D();
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

	mathem::TriangleGeometry3D Mesh::getTrianglePositions(uint32_t index)
	{
		// TODO:

		return mathem::TriangleGeometry3D();
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