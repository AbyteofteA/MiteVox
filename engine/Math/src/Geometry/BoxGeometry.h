#ifndef BOXGEOMETRY_H
#define BOXGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/TriangleGeometry.h"

namespace mathem
{
	class BoxGeometry :
		public GeometryPrimitiveBase,
		public GeometryTransform
	{
	public:

		Point3D halfSize = { 0.5f, 0.5f, 0.5f };

		explicit BoxGeometry();
		bool isTriangularMesh();
		uint32_t getPointsCount();
		Vector3D getPoint(uint32_t index);
		Vector3D getPoint(uint32_t index, GeometryTransform* globalTransform);
		uint32_t getTrianglesCount();
		TriangleGeometry getTriangle(uint32_t index);
		TriangleGeometry getTriangle(uint32_t index, GeometryTransform* globalTransform);
		bool isIdealGeometry();
	};
}

#endif