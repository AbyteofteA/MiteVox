#ifndef TRUNCATEDPYRAMIDGEOMETRY_H
#define TRUNCATEDPYRAMIDGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class TruncatedPyramidGeometry :
		public GeometryPrimitiveBase,
		public GeometryTransform
	{
	public:

		// TODO

		explicit TruncatedPyramidGeometry();
	};
}

#endif

