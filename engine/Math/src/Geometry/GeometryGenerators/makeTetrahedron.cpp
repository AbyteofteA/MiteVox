#include "makeTetrahedron.h"

#include "engine/Math/src/Geometry/GeometryPrimitives/PlaneGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"

namespace mathem
{
	bool makeTetrahedron(
		Vector3D point1,
		Vector3D point2,
		Vector3D point3,
		Vector3D point4,
		mitevox::MeshPrimitive* resultMeshPrimitive)
	{
		// TODO: check if volume is zero!

		resultMeshPrimitive->initIndexedTriangles(4, mitevox::UNSIGNED_SHORT, true);
		resultMeshPrimitive->reserveIndexedTriangles(4);
		resultMeshPrimitive->resizeVerteces(4);
		resultMeshPrimitive->setVertexPosition(0, point1);
		resultMeshPrimitive->setVertexPosition(1, point2);
		resultMeshPrimitive->setVertexPosition(2, point3);
		resultMeshPrimitive->setVertexPosition(3, point4);

		// Triangle 1

		TriangleGeometry3D triangle;
		triangle.getPoint1() = point1;
		triangle.getPoint2() = point2;
		triangle.getPoint3() = point3;
		PlaneGeometry plane = triangle.computePlane();
		if (plane.isBeforePlane(point4))
		{
			resultMeshPrimitive->appendIndexedTriangle(0, 2, 1);
		}
		else
		{
			resultMeshPrimitive->appendIndexedTriangle(0, 1, 2);
		}

		// Triangle 2

		triangle.getPoint1() = point2;
		triangle.getPoint2() = point3;
		triangle.getPoint3() = point4;
		plane = triangle.computePlane();
		if (plane.isBeforePlane(point1))
		{
			resultMeshPrimitive->appendIndexedTriangle(1, 3, 2);
		}
		else
		{
			resultMeshPrimitive->appendIndexedTriangle(1, 2, 3);
		}

		// Triangle 3

		triangle.getPoint1() = point3;
		triangle.getPoint2() = point4;
		triangle.getPoint3() = point1;
		plane = triangle.computePlane();
		if (plane.isBeforePlane(point2))
		{
			resultMeshPrimitive->appendIndexedTriangle(2, 0, 3);
		}
		else
		{
			resultMeshPrimitive->appendIndexedTriangle(2, 3, 0);
		}

		// Triangle 4

		triangle.getPoint1() = point4;
		triangle.getPoint2() = point1;
		triangle.getPoint3() = point2;
		plane = triangle.computePlane();
		if (plane.isBeforePlane(point3))
		{
			resultMeshPrimitive->appendIndexedTriangle(3, 1, 0);
		}
		else
		{
			resultMeshPrimitive->appendIndexedTriangle(3, 0, 1);
		}

		resultMeshPrimitive->tryGenerateSmoothNormalsForIndexedTriangles();

		return true;
	}
}