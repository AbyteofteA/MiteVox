#include "TriangleGeometry2D.h"

namespace mathem
{
	TriangleGeometry2D::TriangleGeometry2D()
	{

	}

	TriangleGeometry2D::TriangleGeometry2D(Vector2D point1, Vector2D point2, Vector2D point3)
	{
		this->points[0] = point1;
		this->points[1] = point2;
		this->points[2] = point3;
	}

	Vector2D& TriangleGeometry2D::getPoint1()
	{
		return points[0];
	}

	Vector2D& TriangleGeometry2D::getPoint2()
	{
		return points[1];
	}

	Vector2D& TriangleGeometry2D::getPoint3()
	{
		return points[2];
	}
}