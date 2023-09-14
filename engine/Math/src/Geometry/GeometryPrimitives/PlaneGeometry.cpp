#include "PlaneGeometry.h"

#include "engine/Math/src/NumericalAnalysis/Intertolation.h"

namespace mathem
{
	float PlaneGeometry::getSignedDistance(Vector3D vertex)
	{
		float vertexProjection = vertex * normal;
		return vertexProjection - position;
	}

	bool PlaneGeometry::isBeforePlane(Vector3D vertex)
	{
		if (getSignedDistance(vertex) > 0.0f)
		{
			return true;
		}
		return false;
	}

	Vector3D PlaneGeometry::projectOntoPlaneIfBehind(Vector3D vertex)
	{
		float vertexProjection = vertex * normal;
		if (vertexProjection >= position)
		{
			return vertex;
		}
		Vector3D difference = normal * (position - vertexProjection);
		return vertex + difference;
	}

	size_t PlaneGeometry::sliceTriangle(
		TriangleGeometry3D triangle, 
		TriangleGeometry3D* resultTrianglesBehindPlane, 
		TriangleGeometry3D* resultTrianglesBeforePlane)
	{
		bool vertexIsBeforePlane1 = isBeforePlane(triangle.getPoint1());
		bool vertexIsBeforePlane2 = isBeforePlane(triangle.getPoint2());
		bool vertexIsBeforePlane3 = isBeforePlane(triangle.getPoint3());

		return sliceTriangle(
			triangle, 
			vertexIsBeforePlane1, 
			vertexIsBeforePlane2, 
			vertexIsBeforePlane3, 
			resultTrianglesBehindPlane, 
			resultTrianglesBeforePlane);
	}

	size_t PlaneGeometry::sliceTriangle(
		TriangleGeometry3D triangle,
		bool vertexIsBeforePlane1,
		bool vertexIsBeforePlane2,
		bool vertexIsBeforePlane3,
		TriangleGeometry3D* resultTrianglesBehindPlane,
		TriangleGeometry3D* resultTrianglesBeforePlane)
	{
		if ((vertexIsBeforePlane1 == vertexIsBeforePlane2) &&
			(vertexIsBeforePlane2 == vertexIsBeforePlane3) &&
			(vertexIsBeforePlane3 == vertexIsBeforePlane1))
		{
			return 0;
		}

		Vector3D trianglesVerteces[2][4];
		size_t vertecesCount[2] = { 0, 0 };

		const size_t BEHIND_PLANE = 0;
		const size_t BEFORE_PLANE = 1;

		// Append the 1st original vertex to the corresponding side of the plane
		trianglesVerteces[vertexIsBeforePlane1][vertecesCount[vertexIsBeforePlane1]] = triangle.getPoint1();
		vertecesCount[vertexIsBeforePlane1] += 1;

		// If there is an intersection between the 1st and the 2nd, append intersection point to both sides of the plane
		if (vertexIsBeforePlane1 != vertexIsBeforePlane2)
		{
			float projection1 = triangle.getPoint1() * normal;
			float projection2 = triangle.getPoint2() * normal;
			float intersectionFraction = (position - projection1) / (projection2 - projection1);
			Vector3D intersectionPoint = lerp(triangle.getPoint1(), triangle.getPoint2(), intersectionFraction);

			trianglesVerteces[BEHIND_PLANE][vertecesCount[BEHIND_PLANE]] = intersectionPoint;
			vertecesCount[BEHIND_PLANE] += 1;
			trianglesVerteces[BEFORE_PLANE][vertecesCount[BEFORE_PLANE]] = intersectionPoint;
			vertecesCount[BEFORE_PLANE] += 1;
		}

		// Append the 2nd original vertex to the corresponding side of the plane
		trianglesVerteces[vertexIsBeforePlane2][vertecesCount[vertexIsBeforePlane2]] = triangle.getPoint2();
		vertecesCount[vertexIsBeforePlane2] += 1;

		// If there is an intersection between the 2nd and the 3rd, append intersection point to both sides of the plane
		if (vertexIsBeforePlane2 != vertexIsBeforePlane3)
		{
			float projection2 = triangle.getPoint2() * normal;
			float projection3 = triangle.getPoint3() * normal;
			float intersectionFraction = (position - projection2) / (projection3 - projection2);
			Vector3D intersectionPoint = lerp(triangle.getPoint2(), triangle.getPoint3(), intersectionFraction);

			trianglesVerteces[BEHIND_PLANE][vertecesCount[BEHIND_PLANE]] = intersectionPoint;
			vertecesCount[BEHIND_PLANE] += 1;
			trianglesVerteces[BEFORE_PLANE][vertecesCount[BEFORE_PLANE]] = intersectionPoint;
			vertecesCount[BEFORE_PLANE] += 1;
		}

		// Append the 3rd original vertex to the corresponding side of the plane
		trianglesVerteces[vertexIsBeforePlane3][vertecesCount[vertexIsBeforePlane3]] = triangle.getPoint3();
		vertecesCount[vertexIsBeforePlane3] += 1;

		// If there is an intersection between the 3rd and the 1st, append intersection point to both sides of the plane
		if (vertexIsBeforePlane3 != vertexIsBeforePlane1)
		{
			float projection3 = triangle.getPoint3() * normal;
			float projection1 = triangle.getPoint1() * normal;
			float intersectionFraction = (position - projection3) / (projection1 - projection3);
			Vector3D intersectionPoint = lerp(triangle.getPoint3(), triangle.getPoint1(), intersectionFraction);

			trianglesVerteces[BEHIND_PLANE][vertecesCount[BEHIND_PLANE]] = intersectionPoint;
			vertecesCount[BEHIND_PLANE] += 1;
			trianglesVerteces[BEFORE_PLANE][vertecesCount[BEFORE_PLANE]] = intersectionPoint;
			vertecesCount[BEFORE_PLANE] += 1;
		}


		if (vertecesCount[BEHIND_PLANE] == 3)
		{
			resultTrianglesBehindPlane[0].getPoint1() = trianglesVerteces[BEHIND_PLANE][0];
			resultTrianglesBehindPlane[0].getPoint2() = trianglesVerteces[BEHIND_PLANE][1];
			resultTrianglesBehindPlane[0].getPoint3() = trianglesVerteces[BEHIND_PLANE][2];
		}
		else
		{
			resultTrianglesBehindPlane[0].getPoint1() = trianglesVerteces[BEHIND_PLANE][0];
			resultTrianglesBehindPlane[0].getPoint2() = trianglesVerteces[BEHIND_PLANE][1];
			resultTrianglesBehindPlane[0].getPoint3() = trianglesVerteces[BEHIND_PLANE][2];
			resultTrianglesBehindPlane[1].getPoint1() = trianglesVerteces[BEHIND_PLANE][0];
			resultTrianglesBehindPlane[1].getPoint2() = trianglesVerteces[BEHIND_PLANE][2];
			resultTrianglesBehindPlane[1].getPoint3() = trianglesVerteces[BEHIND_PLANE][3];
		}

		if (vertecesCount[BEFORE_PLANE] == 3)
		{
			resultTrianglesBeforePlane[0].getPoint1() = trianglesVerteces[BEFORE_PLANE][0];
			resultTrianglesBeforePlane[0].getPoint2() = trianglesVerteces[BEFORE_PLANE][1];
			resultTrianglesBeforePlane[0].getPoint3() = trianglesVerteces[BEFORE_PLANE][2];
		}
		else
		{
			resultTrianglesBeforePlane[0].getPoint1() = trianglesVerteces[BEFORE_PLANE][0];
			resultTrianglesBeforePlane[0].getPoint2() = trianglesVerteces[BEFORE_PLANE][1];
			resultTrianglesBeforePlane[0].getPoint3() = trianglesVerteces[BEFORE_PLANE][2];
			resultTrianglesBeforePlane[1].getPoint1() = trianglesVerteces[BEFORE_PLANE][0];
			resultTrianglesBeforePlane[1].getPoint2() = trianglesVerteces[BEFORE_PLANE][2];
			resultTrianglesBeforePlane[1].getPoint3() = trianglesVerteces[BEFORE_PLANE][3];
		}

		return vertecesCount[BEHIND_PLANE];
	}
}