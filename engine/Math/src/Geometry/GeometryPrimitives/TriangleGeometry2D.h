#ifndef TRIANGLEGEOMETRY2D_H
#define TRIANGLEGEOMETRY2D_H

#include "engine/Math/src/Vector.h"

namespace mathem
{
	class TriangleGeometry2D
	{
	public:

		Vector2D point1;
		Vector2D point2;
		Vector2D point3;

		explicit TriangleGeometry2D();
		explicit TriangleGeometry2D(Vector2D point1, Vector2D point2, Vector2D point3);
	};
}

#endif