
#ifndef TRIANGLE_GEOMETRY_H
#define TRIANGLE_GEOMETRY_H

#include "engine/Math/src/Vector.h"

namespace mathem
{
	class TriangleGeometry
	{
	public:

		Vector3D point1;
		Vector3D point2;
		Vector3D point3;

		explicit TriangleGeometry();
		explicit TriangleGeometry(Vector3D point1, Vector3D point2, Vector3D point3);
		Vector3D computeNormal();
	};
}

#endif