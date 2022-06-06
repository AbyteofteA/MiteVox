
#ifndef MITEVOX_TRIANGLE_H
#define MITEVOX_TRIANGLE_H

#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Point3D.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	class TriangleGeometry : public GeometryPrimitiveBase
	{
	public:

		Vector3D point1;
		Vector3D point2;
		Vector3D point3;

		explicit TriangleGeometry();
		explicit TriangleGeometry(Vector3D point1, Vector3D point2, Vector3D point3);
		Vector3D computeNormal();

		bool isTriangularMesh();
		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry getTrianglePositions(uint32_t index);
		bool isIdealGeometry();
	};
}

#endif