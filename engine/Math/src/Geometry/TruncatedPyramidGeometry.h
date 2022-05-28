#ifndef TRUNCATEDPYRAMIDGEOMETRY_H
#define TRUNCATEDPYRAMIDGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/TriangleGeometry.h"

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

