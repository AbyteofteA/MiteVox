#ifndef TRIANGLEGEOMETRY3D_H
#define TRIANGLEGEOMETRY3D_H

#include "engine/Math/src/Vector.h"

namespace mathem
{
	class TriangleGeometry3D
	{
	public:

		Vector3D point1;
		Vector3D point2;
		Vector3D point3;

		explicit TriangleGeometry3D();
		explicit TriangleGeometry3D(Vector3D point1, Vector3D point2, Vector3D point3);
		Vector3D computeNormal();
	};
}

#endif