#include "TriangleGeometry3D.h"

#include "engine/Math/src/Geometry/GeometryPrimitives/PlaneGeometry.h"

namespace mathem
{
	TriangleGeometry3D::TriangleGeometry3D()
	{
		this->points[0] = Vector3D::zero();
		this->points[1] = Vector3D::zero();
		this->points[2] = Vector3D::zero();
	}

	TriangleGeometry3D::TriangleGeometry3D(Vector3D point1, Vector3D point2, Vector3D point3)
	{
		this->points[0] = point1;
		this->points[1] = point2;
		this->points[2] = point3;
	}

	Vector3D& TriangleGeometry3D::getPoint1()
	{
		return points[0];
	}

	Vector3D& TriangleGeometry3D::getPoint2()
	{
		return points[1];
	}

	Vector3D& TriangleGeometry3D::getPoint3()
	{
		return points[2];
	}

	float TriangleGeometry3D::computePerimeter()
	{
		Vector3D vectorA = points[1] - points[0];
		Vector3D vectorB = points[2] - points[0];
		Vector3D vectorC = points[2] - points[1];
		return vectorA.getLength() + vectorB.getLength() + vectorC.getLength();
	}

	Vector3D TriangleGeometry3D::computeCentroid()
	{
		Vector3D centroid = points[0] + points[1] + points[2];
		return centroid / 3.0f;
	}

	Vector3D TriangleGeometry3D::computeNormal()
	{
		Vector3D vectorA = points[1] - points[0];
		Vector3D vectorB = points[2] - points[0];

		Vector3D normal = crossProduct(vectorA, vectorB);
		normal.normalize();
		return normal;
	}

	PlaneGeometry TriangleGeometry3D::computePlane()
	{
		PlaneGeometry plane;
		plane.normal = computeNormal();
		plane.position = std::max(plane.normal * points[0], plane.normal * points[1]);
		plane.position = std::max(plane.position, plane.normal * points[2]);
		return plane;
	}
}