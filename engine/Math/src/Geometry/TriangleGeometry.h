
#ifndef MITEVOX_TRIANGLE_H
#define MITEVOX_TRIANGLE_H

#include "engine/Math/src/Geometry/GeometryPrimitiveBase.h"
#include "engine/Math/src/LinearAlgebra/Point3D.h"
#include "engine/Math/src/LinearAlgebra/Vector3D.h"

namespace mathem
{
	class TriangleGeometry : public GeometryPrimitiveBase
	{
	public:

		Vector3D point1 = { 0.f };
		Vector3D point2 = { 0.f };
		Vector3D point3 = { 0.f };

		explicit TriangleGeometry();
		explicit TriangleGeometry(Vector3D point1, Vector3D point2, Vector3D point3);
		Vector3D computeNormal();

		bool isTriangularMesh();
		uint32_t getPointsCount();
		Vector3D getPoint(uint32_t index);
		Vector3D getPoint(uint32_t index, GeometryTransform* globalTransform);
		uint32_t getTrianglesCount();
		TriangleGeometry getTriangle(uint32_t index);
		TriangleGeometry getTriangle(uint32_t index, GeometryTransform* globalTransform);
		bool isIdealGeometry();
	};
}

#endif