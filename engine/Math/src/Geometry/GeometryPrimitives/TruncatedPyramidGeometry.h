#ifndef TRUNCATEDPYRAMIDGEOMETRY_H
#define TRUNCATEDPYRAMIDGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

namespace mathem
{
	class TruncatedPyramidGeometry : public GeometryPrimitiveBase
	{
	public:

		GeometryTransform transform;
		float FOV = 45.f;
		float halfWidth, halfHeight;
		float nearPlane = 0.1f;
		float farPlane = 100000.f;

		explicit TruncatedPyramidGeometry();
		bool isTriangularMesh();
		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry3D getTrianglePositions(uint32_t index);
		bool isIdealGeometry();
	};
}

#endif

