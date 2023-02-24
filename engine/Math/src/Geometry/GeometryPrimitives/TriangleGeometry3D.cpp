#include "TriangleGeometry3D.h"

namespace mathem
{
	TriangleGeometry3D::TriangleGeometry3D()
	{
		
	}

	TriangleGeometry3D::TriangleGeometry3D(Vector3D point1, Vector3D point2, Vector3D point3)
	{
		this->point1 = point1;
		this->point2 = point2;
		this->point3 = point3;
	}

	Vector3D TriangleGeometry3D::computeNormal()
	{
		Vector3D vectorA = point2 - point1;
		Vector3D vectorB = point3 - point1;

		Vector3D normal = crossProduct(vectorA, vectorB);
		normal.normalize();
		return normal;
	}
}