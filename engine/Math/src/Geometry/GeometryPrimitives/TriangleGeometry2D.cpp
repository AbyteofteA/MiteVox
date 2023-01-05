#include "TriangleGeometry2D.h"

namespace mathem
{
	TriangleGeometry2D::TriangleGeometry2D()
	{

	}

	TriangleGeometry2D::TriangleGeometry2D(Vector2D point1, Vector2D point2, Vector2D point3)
	{
		this->point1 = point1;
		this->point2 = point2;
		this->point3 = point3;
	}
}