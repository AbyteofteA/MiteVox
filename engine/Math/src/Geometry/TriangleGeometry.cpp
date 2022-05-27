#include "TriangleGeometry.h"

namespace mathem
{
	TriangleGeometry::TriangleGeometry()
	{
		type = GeometryPrimitiveType::TRIANGLE;
	}

	TriangleGeometry::TriangleGeometry(Vector3D point1, Vector3D point2, Vector3D point3)
	{
		this->point1 = point1;
		this->point2 = point2;
		this->point3 = point3;
	}

	Vector3D TriangleGeometry::computeNormal()
	{
		Vector3D vectorA = point3 - point2;
		Vector3D vectorB = point1 - point2;

		Vector3D normal = crossProduct(vectorA, vectorB);
		normal.normalize();
		return normal;
	}

	bool TriangleGeometry::isTriangularMesh()
	{
		return true;
	}

	uint32_t TriangleGeometry::getPointsCount()
	{
		return 3;
	}

	Vector3D TriangleGeometry::getPoint(uint32_t index)
	{
		switch (index)
		{
		case 0:
			return point1;
		case 1:
			return point2;
		case 2:
			return point3;
		default:
			break;
		}
		return Vector3D();
	}

	Vector3D TriangleGeometry::getPoint(uint32_t index, GeometryTransform* globalTransform)
	{
		Vector3D resultPoint = getPoint(index);
		// TODO: transform resultPoint via globalTransform;
		return resultPoint;
	}

	uint32_t TriangleGeometry::getTrianglesCount()
	{
		return 1;
	}

	TriangleGeometry TriangleGeometry::getTriangle(uint32_t index)
	{
		return *this;
	}

	TriangleGeometry TriangleGeometry::getTriangle(uint32_t index, GeometryTransform* globalTransform)
	{
		TriangleGeometry resultTriangle = *this;
		// TODO: transform resultTriangle via globalTransform;
		return resultTriangle;
	}

	bool TriangleGeometry::isIdealGeometry()
	{
		return false;
	}
}