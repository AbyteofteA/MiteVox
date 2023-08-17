#include "SphereGeometry.h"

namespace mathem
{
	SphereGeometry::SphereGeometry()
	{
		type = GeometryPrimitiveType::SPHERE;
	}

	uint32_t SphereGeometry::getVertecesCount()
	{
		return 0;
	}

	Vector3D SphereGeometry::getVertexPosition(uint32_t index)
	{
		return Vector3D();
	}

	uint32_t SphereGeometry::getTrianglesCount()
	{
		return 0;
	}

	TriangleGeometry3D SphereGeometry::getTrianglePositions(uint32_t index)
	{
		return TriangleGeometry3D();
	}
}