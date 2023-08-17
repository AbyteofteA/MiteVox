#include "CapsuleGeometry.h"

namespace mathem
{
	CapsuleGeometry::CapsuleGeometry()
	{
		type = GeometryPrimitiveType::CAPSULE;
	}

	uint32_t CapsuleGeometry::getVertecesCount()
	{
		return 0;
	}

	Vector3D CapsuleGeometry::getVertexPosition(uint32_t index)
	{
		return Vector3D();
	}

	uint32_t CapsuleGeometry::getTrianglesCount()
	{
		return 0;
	}

	TriangleGeometry3D CapsuleGeometry::getTrianglePositions(uint32_t index)
	{
		return TriangleGeometry3D();
	}
}