#ifndef SPHEREGEOMETRY_H
#define SPHEREGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class SphereGeometry : public GeometryPrimitiveBase
	{
	public:

		GeometryTransform transform;
		float radius = 0.5f;

		explicit SphereGeometry();
		bool isTriangularMesh();
		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry getTrianglePositions(uint32_t index);
		bool isIdealGeometry();
	};
}

#endif