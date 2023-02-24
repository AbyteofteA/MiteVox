#ifndef AXISALIGNEDBOXGEOMETRY_H
#define AXISALIGNEDBOXGEOMETRY_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

namespace mathem
{
	class AxisAlignedBoxGeometry : public GeometryPrimitiveBase
	{
	public:

		Vector3D position;
		Vector3D halfSize = { 0.5f, 0.5f, 0.5f };

		explicit AxisAlignedBoxGeometry();
		bool isTriangularMesh();
		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry3D getTrianglePositions(uint32_t index);
		bool isIdealGeometry();
	};
}

#endif