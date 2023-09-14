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

		/// <summary>
		/// 
		/// </summary>
		/// <param name="triangle"> - triangle to slice</param>
		/// <param name="resultTrianglesBehindPlane"> - output array of 2 triangles</param>
		/// <param name="resultTrianglesBeforePlane"> - output array of 2 triangles</param>
		/// <returns>0 if nothing to slice, result triangles count behind the plane otherwise</returns>
		size_t sliceTriangle(
			TriangleGeometry3D triangle, 
			TriangleGeometry3D* resultTrianglesBehindPlane, 
			TriangleGeometry3D* resultTrianglesBeforePlane);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="triangle"> - triangle to slice</param>
		/// <param name="vertexIsBeforePlane1"></param>
		/// <param name="vertexIsBeforePlane2"></param>
		/// <param name="vertexIsBeforePlane3"></param>
		/// <param name="resultTrianglesBehindPlane"> - output array of 2 triangles</param>
		/// <param name="resultTrianglesBeforePlane"> - output array of 2 triangles</param>
		/// <returns>0 if nothing to slice, result triangles count behind the plane otherwise</returns>
		size_t sliceTriangle(
			TriangleGeometry3D triangle, 
			bool vertexIsBeforePlane1,
			bool vertexIsBeforePlane2,
			bool vertexIsBeforePlane3,
			TriangleGeometry3D* resultTrianglesBehindPlane,
			TriangleGeometry3D* resultTrianglesBeforePlane);
	};
}

#endif

