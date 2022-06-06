#ifndef SPHEREGEOMETRY_H
#define SPHEREGEOMETRY_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"

namespace mathem
{
	class SphereGeometry :
		public GeometryPrimitiveBase,
		public GeometryTransform
	{
	public:

		float radius = 0.5f;

		explicit SphereGeometry();
	};
}

#endif