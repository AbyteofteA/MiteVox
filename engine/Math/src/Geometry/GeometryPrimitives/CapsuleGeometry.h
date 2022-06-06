#ifndef CAPSULEGEOMETRY_H
#define CAPSULEGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class CapsuleGeometry :
		public GeometryPrimitiveBase,
		public GeometryTransform
	{
	public:

		float halfHeight = 0.5f;
		float radius = 0.5f;

		explicit CapsuleGeometry();
	};
}

#endif

