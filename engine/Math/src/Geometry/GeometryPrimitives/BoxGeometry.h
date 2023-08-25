#ifndef BOXGEOMETRY_H
#define BOXGEOMETRY_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

namespace mathem
{
	class BoxGeometry
	{
	public:

		Vector3D halfSize = { 0.5f, 0.5f, 0.5f };
		GeometryTransform transform;

		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry3D getTrianglePositions(uint32_t index);
	};
}

#endif