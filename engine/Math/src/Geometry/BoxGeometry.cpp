#include "BoxGeometry.h"

namespace mathem
{
	BoxGeometry::BoxGeometry()
	{
		type = GeometryPrimitiveType::BOX;
	}

	bool BoxGeometry::isTriangularMesh()
	{
		return true;
	}

	uint32_t BoxGeometry::getPointsCount()
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
	Vector3D BoxGeometry::getPoint(uint32_t index)
	{
		Vector3D resultPoint;

		switch (index)
		{
		case 0:
			resultPoint = { -halfSize.x, -halfSize.y, halfSize.z };
		case 1:
			resultPoint = { -halfSize.x, halfSize.y, halfSize.z };
		case 2:
			resultPoint = { halfSize.x, halfSize.y, halfSize.z };
		case 3:
			resultPoint = { halfSize.x, -halfSize.y, halfSize.z };
		case 4:
			resultPoint = { halfSize.x, halfSize.y, -halfSize.z };
		case 5:
			resultPoint = { halfSize.x, -halfSize.y, -halfSize.z };
		case 6:
			resultPoint = { -halfSize.x, halfSize.y, -halfSize.z };
		case 7:
			resultPoint = { -halfSize.x, -halfSize.y, -halfSize.z };
		default:
			resultPoint = Vector3D();
		}

		/// TODO: transform resultPoint

		return resultPoint;
	}

	Vector3D BoxGeometry::getPoint(uint32_t index, GeometryTransform* globalTransform)
	{
		Vector3D resultPoint = getPoint(index);
		// TODO: transform resultPoint via globalTransform;
		return resultPoint;
	}

	uint32_t BoxGeometry::getTrianglesCount()
	{
		return 12;
	}

	TriangleGeometry BoxGeometry::getTriangle(uint32_t index)
	{
		TriangleGeometry resultTriangle;

		if (index >= 12)
		{
			return resultTriangle;
		}

		if (index == 0)
		{
			resultTriangle.point1 = getPoint(0);
			resultTriangle.point2 = getPoint(1);
			resultTriangle.point3 = getPoint(2);
		}
		else if (index == 1)
		{
			resultTriangle.point1 = getPoint(2);
			resultTriangle.point2 = getPoint(3);
			resultTriangle.point3 = getPoint(0);
		}
		else if (index == 2)
		{
			resultTriangle.point1 = getPoint(3);
			resultTriangle.point2 = getPoint(2);
			resultTriangle.point3 = getPoint(4);
		}
		else if (index == 3)
		{
			resultTriangle.point1 = getPoint(4);
			resultTriangle.point2 = getPoint(5);
			resultTriangle.point3 = getPoint(3);
		}
		else if (index == 4)
		{
			resultTriangle.point1 = getPoint(1);
			resultTriangle.point2 = getPoint(6);
			resultTriangle.point3 = getPoint(4);
		}
		else if (index == 5)
		{
			resultTriangle.point1 = getPoint(4);
			resultTriangle.point2 = getPoint(2);
			resultTriangle.point3 = getPoint(1);
		}
		else if (index == 6)
		{
			resultTriangle.point1 = getPoint(0);
			resultTriangle.point2 = getPoint(7);
			resultTriangle.point3 = getPoint(5);
		}
		else if (index == 7)
		{
			resultTriangle.point1 = getPoint(5);
			resultTriangle.point2 = getPoint(3);
			resultTriangle.point3 = getPoint(0);
		}
		else if (index == 8)
		{
			resultTriangle.point1 = getPoint(0);
			resultTriangle.point2 = getPoint(1);
			resultTriangle.point3 = getPoint(6);
		}
		else if (index == 9)
		{
			resultTriangle.point1 = getPoint(6);
			resultTriangle.point2 = getPoint(7);
			resultTriangle.point3 = getPoint(0);
		}
		else if (index == 10)
		{
			resultTriangle.point1 = getPoint(7);
			resultTriangle.point2 = getPoint(6);
			resultTriangle.point3 = getPoint(4);
		}
		else if (index == 11)
		{
			resultTriangle.point1 = getPoint(4);
			resultTriangle.point2 = getPoint(5);
			resultTriangle.point3 = getPoint(7);
		}
		
		/// TODO: transform resultTriangle

		return resultTriangle;
	}

	TriangleGeometry BoxGeometry::getTriangle(uint32_t index, GeometryTransform* globalTransform)
	{
		TriangleGeometry resultTriangle = getTriangle(index);
		// TODO: transform resultTriangle via globalTransform;
		return resultTriangle;
	}

	bool BoxGeometry::isIdealGeometry()
	{
		return false;
	}
}