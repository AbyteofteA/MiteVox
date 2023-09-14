#ifndef PLANEGEOMETRY_H
#define PLANEGEOMETRY_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

namespace mathem
{
	class PlaneGeometry
	{
	public:

		float position = 0.0f;
		Vector3D normal = { 0.0f, 1.0f, 0.0f };

		float getSignedDistance(Vector3D vertex);
		bool isBeforePlane(Vector3D vertex);
		Vector3D projectOntoPlaneIfBehind(Vector3D vertex);
	};
}

#endif

