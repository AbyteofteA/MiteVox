#ifndef CAPSULEGEOMETRY_H
#define CAPSULEGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class CapsuleGeometry : public GeometryPrimitiveBase
	{
	public:

		GeometryTransform transform;
		float halfHeight = 0.5f;
		float radius = 0.5f;

		explicit CapsuleGeometry();
		bool isTriangularMesh();
		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry getTrianglePositions(uint32_t index);
		bool isIdealGeometry();
	};
}

#endif

