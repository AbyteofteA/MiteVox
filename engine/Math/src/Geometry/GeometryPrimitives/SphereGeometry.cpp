#include "SphereGeometry.h"

namespace mathem
{
	SphereGeometry::SphereGeometry()
	{
		type = GeometryPrimitiveType::SPHERE;
	}

	bool SphereGeometry::isTriangularMesh()
	{
		return false;
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

	TriangleGeometry SphereGeometry::getTrianglePositions(uint32_t index)
	{
		return TriangleGeometry();
	}

	bool SphereGeometry::isIdealGeometry()
	{
		return true;
	}
}