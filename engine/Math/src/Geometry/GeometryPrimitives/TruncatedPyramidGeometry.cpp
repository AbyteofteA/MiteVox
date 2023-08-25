#include "TruncatedPyramidGeometry.h"

namespace mathem
{
	uint32_t TruncatedPyramidGeometry::getVertecesCount()
	{
		return 8;
	}

	/// <summary>
	/// 
	///   1-----2
	///  /|    /|
	/// 6-----4 |
	/// | 0---|-3   y z
	/// |/    |/    |/
	/// 7-----5     *---x
	/// 
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	Vector3D TruncatedPyramidGeometry::getVertexPosition(uint32_t index)
	{
		Vector3D resultPoint;

		float nearHalfWidth = (halfWidth * nearPlane) / farPlane;
		float nearHalfHeight = (halfHeight * nearPlane) / farPlane;

		switch (index)
		{
		case 0:
			resultPoint = { -halfWidth, -halfHeight, farPlane };
			break;
		case 1:
			resultPoint = { -halfWidth, halfHeight, farPlane };
			break;
		case 2:
			resultPoint = { halfWidth, halfHeight, farPlane };
			break;
		case 3:
			resultPoint = { halfWidth, -halfHeight, farPlane };
			break;
		case 4:
			resultPoint = { nearHalfWidth, nearHalfHeight, nearPlane };
			break;
		case 5:
			resultPoint = { nearHalfWidth, -nearHalfHeight, nearPlane };
			break;
		case 6:
			resultPoint = { -nearHalfWidth, nearHalfHeight, nearPlane };
			break;
		case 7:
			resultPoint = { -nearHalfWidth, -nearHalfHeight, nearPlane };
			break;
		default:
			resultPoint = Vector3D();
			break;
		}

		transform.applyTo(resultPoint);

		return resultPoint;
	}

	uint32_t TruncatedPyramidGeometry::getTrianglesCount()
	{
		return 12;
	}

	TriangleGeometry3D TruncatedPyramidGeometry::getTrianglePositions(uint32_t index)
	{
		TriangleGeometry3D resultTriangle;

		if (index >= 12)
		{
			return resultTriangle;
		}

		if (index == 0)
		{
			resultTriangle.point1 = getVertexPosition(0);
			resultTriangle.point2 = getVertexPosition(1);
			resultTriangle.point3 = getVertexPosition(2);
		}
		else if (index == 1)
		{
			resultTriangle.point1 = getVertexPosition(2);
			resultTriangle.point2 = getVertexPosition(3);
			resultTriangle.point3 = getVertexPosition(0);
		}
		else if (index == 2)
		{
			resultTriangle.point1 = getVertexPosition(3);
			resultTriangle.point2 = getVertexPosition(2);
			resultTriangle.point3 = getVertexPosition(4);
		}
		else if (index == 3)
		{
			resultTriangle.point1 = getVertexPosition(4);
			resultTriangle.point2 = getVertexPosition(5);
			resultTriangle.point3 = getVertexPosition(3);
		}
		else if (index == 4)
		{
			resultTriangle.point1 = getVertexPosition(1);
			resultTriangle.point2 = getVertexPosition(6);
			resultTriangle.point3 = getVertexPosition(4);
		}
		else if (index == 5)
		{
			resultTriangle.point1 = getVertexPosition(4);
			resultTriangle.point2 = getVertexPosition(2);
			resultTriangle.point3 = getVertexPosition(1);
		}
		else if (index == 6)
		{
			resultTriangle.point1 = getVertexPosition(0);
			resultTriangle.point2 = getVertexPosition(7);
			resultTriangle.point3 = getVertexPosition(5);
		}
		else if (index == 7)
		{
			resultTriangle.point1 = getVertexPosition(5);
			resultTriangle.point2 = getVertexPosition(3);
			resultTriangle.point3 = getVertexPosition(0);
		}
		else if (index == 8)
		{
			resultTriangle.point1 = getVertexPosition(0);
			resultTriangle.point2 = getVertexPosition(1);
			resultTriangle.point3 = getVertexPosition(6);
		}
		else if (index == 9)
		{
			resultTriangle.point1 = getVertexPosition(6);
			resultTriangle.point2 = getVertexPosition(7);
			resultTriangle.point3 = getVertexPosition(0);
		}
		else if (index == 10)
		{
			resultTriangle.point1 = getVertexPosition(7);
			resultTriangle.point2 = getVertexPosition(6);
			resultTriangle.point3 = getVertexPosition(4);
		}
		else if (index == 11)
		{
			resultTriangle.point1 = getVertexPosition(4);
			resultTriangle.point2 = getVertexPosition(5);
			resultTriangle.point3 = getVertexPosition(7);
		}

		return resultTriangle;
	}
}