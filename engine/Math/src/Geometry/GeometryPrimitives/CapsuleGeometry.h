#ifndef CAPSULEGEOMETRY_H
#define CAPSULEGEOMETRY_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

namespace mathem
{
	class CapsuleGeometry
	{
	public:

		float halfHeight = 0.5f;
		float radius = 0.5f;
		GeometryTransform transform;

		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry3D getTrianglePositions(uint32_t index);
	};
}

#endif

