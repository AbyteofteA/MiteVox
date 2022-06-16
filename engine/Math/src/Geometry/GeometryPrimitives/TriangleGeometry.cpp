#include "TriangleGeometry.h"

namespace mathem
{
	TriangleGeometry::TriangleGeometry()
	{
		
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
}