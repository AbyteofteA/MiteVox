#ifndef RAYGEOMETRY_H
#define RAYGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/TriangleGeometry.h"

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