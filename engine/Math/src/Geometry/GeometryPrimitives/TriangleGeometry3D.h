#ifndef TRIANGLEGEOMETRY3D_H
#define TRIANGLEGEOMETRY3D_H

#include "engine/Math/src/Vector.h"

namespace mathem
{
	class PlaneGeometry;

	class TriangleGeometry3D // TODO: make it template
	{
	public:

		Vector3D points[3];

		explicit TriangleGeometry3D();
		explicit TriangleGeometry3D(Vector3D point1, Vector3D point2, Vector3D point3);

		Vector3D& getPoint1();
		Vector3D& getPoint2();
		Vector3D& getPoint3();

		float computePerimeter();
		Vector3D computeCentroid();
		Vector3D computeNormal();
		PlaneGeometry computePlane();
	};
}

#endif