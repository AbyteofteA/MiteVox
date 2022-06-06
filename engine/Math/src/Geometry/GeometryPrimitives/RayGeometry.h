#ifndef RAYGEOMETRY_H
#define RAYGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class RayGeometry :
		public GeometryPrimitiveBase,
		public GeometryTransform
	{
	public:

		// TODO

		explicit RayGeometry();
	};
}

#endif