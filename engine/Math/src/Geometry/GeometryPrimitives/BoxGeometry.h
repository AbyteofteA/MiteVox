#ifndef BOXGEOMETRY_H
#define BOXGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class BoxGeometry : public GeometryPrimitiveBase
	{
	public:

		GeometryTransform transform;
		Vector3D halfSize = { 0.5f, 0.5f, 0.5f };

		explicit BoxGeometry();
		bool isTriangularMesh();
		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry getTrianglePositions(uint32_t index);
		bool isIdealGeometry();
	};
}

#endif