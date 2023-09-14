#include "GeometryPrimitiveBase.h"

#include "engine/Math/src/Utilities/almostEqual.h"
#include "engine/Math/src/Utilities/MinAndMax.h"

namespace mathem
{
	AnyGeometryPrimitive::AnyGeometryPrimitive()
	{
		AABB = AxisAlignedBoxGeometry();
	}

	GeometryPrimitiveBase GeometryPrimitiveBase::axisAlignedBoxToBox()
	{
		AxisAlignedBoxGeometry* AABBData = getAxisAlignedBox();
		GeometryPrimitiveBase box;
		GeometryTransform transform;
		transform.translation = AABBData->position;
		box.setBox(AABBData->halfSize, transform);
		return box;
	}

	Point* GeometryPrimitiveBase::getPoint()
	{
		return &anyPrimitive.point;
	}

	Line* GeometryPrimitiveBase::getLine()
	{
		return &anyPrimitive.line;
	}

	Ray* GeometryPrimitiveBase::getRay()
	{
		return &anyPrimitive.ray;
	}

	PlaneGeometry* GeometryPrimitiveBase::getPlane()
	{
		return &anyPrimitive.plane;
	}

	AxisAlignedBoxGeometry* GeometryPrimitiveBase::getAxisAlignedBox()
	{
		return &anyPrimitive.AABB;
	}

	BoxGeometry* GeometryPrimitiveBase::getBox()
	{
		return &anyPrimitive.box;
	}

	SphereGeometry* GeometryPrimitiveBase::getSphere()
	{
		return &anyPrimitive.sphere;
	}

	CapsuleGeometry* GeometryPrimitiveBase::getCapsule()
	{
		return &anyPrimitive.capsule;
	}

	TruncatedPyramidGeometry* GeometryPrimitiveBase::getTruncatedPyramid()
	{
		return &anyPrimitive.truncatedPyramid;
	}

	mitevox::MeshPrimitive* GeometryPrimitiveBase::getConvexHull()
	{
		return anyPrimitive.convexHull;
	}

	void GeometryPrimitiveBase::setPoint(Vector3D position)
	{
		type = mathem::GeometryPrimitiveType::POINT;
		anyPrimitive.point.position = position;
	}

	void GeometryPrimitiveBase::setLine(Vector3D point1, Vector3D point2)
	{
		type = mathem::GeometryPrimitiveType::LINE;
		anyPrimitive.line.points[0] = point1;
		anyPrimitive.line.points[1] = point2;
	}

	void GeometryPrimitiveBase::setRay(Vector3D position, Vector3D direction)
	{
		type = mathem::GeometryPrimitiveType::RAY;
		anyPrimitive.ray.position = position;
		anyPrimitive.ray.direction = direction;
	}

	void GeometryPrimitiveBase::setPlane(float position, Vector3D normal)
	{
		type = mathem::GeometryPrimitiveType::PLANE;
		anyPrimitive.plane.position = position;
		anyPrimitive.plane.normal = normal;
	}

	void GeometryPrimitiveBase::setAxisAlignedBox(Vector3D position, Vector3D halfSize)
	{
		type = mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX;
		anyPrimitive.AABB.position = position;
		anyPrimitive.AABB.halfSize = halfSize;
	}

	void GeometryPrimitiveBase::setBox()
	{
		type = mathem::GeometryPrimitiveType::BOX;
		anyPrimitive.box = BoxGeometry();
	}

	void GeometryPrimitiveBase::setBox(Vector3D halfSize, Vector3D translation)
	{
		type = mathem::GeometryPrimitiveType::BOX;
		anyPrimitive.box.halfSize = halfSize;
		GeometryTransform transform;
		transform.translation = translation;
		anyPrimitive.box.transform = transform;
	}

	void GeometryPrimitiveBase::setBox(Vector3D halfSize, GeometryTransform transform)
	{
		type = mathem::GeometryPrimitiveType::BOX;
		anyPrimitive.box.halfSize = halfSize;
		anyPrimitive.box.transform = transform;
	}

	void GeometryPrimitiveBase::setSphere()
	{
		type = mathem::GeometryPrimitiveType::SPHERE;
		anyPrimitive.sphere = SphereGeometry();
	}

	void GeometryPrimitiveBase::setSphere(float radius, GeometryTransform transform)
	{
		type = mathem::GeometryPrimitiveType::SPHERE;
		anyPrimitive.sphere.radius = radius;
		anyPrimitive.sphere.transform = transform;
	}

	void GeometryPrimitiveBase::setCapsule()
	{
		type = mathem::GeometryPrimitiveType::CAPSULE;
		anyPrimitive.capsule = CapsuleGeometry();
	}

	void GeometryPrimitiveBase::setCapsule(float halfHeight, float radius, GeometryTransform transform)
	{
		type = mathem::GeometryPrimitiveType::CAPSULE;
		anyPrimitive.capsule.halfHeight = halfHeight;
		anyPrimitive.capsule.radius = radius;
		anyPrimitive.capsule.transform = transform;
	}

	void GeometryPrimitiveBase::setTruncatedPyramid()
	{
		type = mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID;
		anyPrimitive.truncatedPyramid = TruncatedPyramidGeometry();
	}

	void GeometryPrimitiveBase::setTruncatedPyramid(float FOV, float halfWidth, float halfHeight, float nearPlane, float farPlane, GeometryTransform transform)
	{
		type = mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID;
		anyPrimitive.truncatedPyramid.FOV = FOV;
		anyPrimitive.truncatedPyramid.halfWidth = halfWidth;
		anyPrimitive.truncatedPyramid.halfHeight = halfHeight;
		anyPrimitive.truncatedPyramid.nearPlane = nearPlane;
		anyPrimitive.truncatedPyramid.farPlane = farPlane;
		anyPrimitive.truncatedPyramid.transform = transform;
	}

	void GeometryPrimitiveBase::setConvexHull(mitevox::MeshPrimitive* convexHull)
	{
		type = mathem::GeometryPrimitiveType::CONVEX_HULL;
		anyPrimitive.convexHull = convexHull;
	}

	float GeometryPrimitiveBase::getVolume()
	{
		switch (type)
		{

		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
		{
			Vector3D AABBSize = anyPrimitive.AABB.halfSize * 2.0f;
			return AABBSize.x() * AABBSize.y() * AABBSize.z();
		}
		case mathem::GeometryPrimitiveType::BOX:
		{
			Vector3D boxSize = anyPrimitive.box.halfSize * 2.0f;
			return boxSize.x() * boxSize.y() * boxSize.z();
		}
		case mathem::GeometryPrimitiveType::SPHERE:
			// TODO:
			return 1.0f;
		case mathem::GeometryPrimitiveType::CAPSULE:
			// TODO:
			return 1.0f;
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			// TODO:
			return 1.0f;
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			// TODO:
			return 1.0f;

		case mathem::GeometryPrimitiveType::NONE:
		case mathem::GeometryPrimitiveType::POINT:
		case mathem::GeometryPrimitiveType::LINE:
		case mathem::GeometryPrimitiveType::RAY:
		case mathem::GeometryPrimitiveType::PLANE:
		default:
			break;
		}
		return 0.0f;
	}

	GeometryPrimitiveType GeometryPrimitiveBase::getType()
	{
		return type;
	}

	bool GeometryPrimitiveBase::isTriangularMesh()
	{
		switch (type)
		{
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
		case mathem::GeometryPrimitiveType::BOX:
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			return true;

		case mathem::GeometryPrimitiveType::NONE:
		case mathem::GeometryPrimitiveType::POINT:
		case mathem::GeometryPrimitiveType::LINE:
		case mathem::GeometryPrimitiveType::RAY:
		case mathem::GeometryPrimitiveType::PLANE:
		case mathem::GeometryPrimitiveType::SPHERE:
		case mathem::GeometryPrimitiveType::CAPSULE:
		default:
			return false;
		}
	}

	bool containsVertex(Vector3D vertex, Vector3D* faceVerteces, size_t faceVertecesCount)
	{
		for (size_t i = 0; i < faceVertecesCount; ++i)
		{
			if (faceVerteces[i] == vertex)
			{
				return true;
			}
		}
		return false;
	}

	void tryAppendVertex(Vector3D vertex, Vector3D* faceVerteces, size_t* faceVertecesCount, float equalityTolerance)
	{
		// Check if contains the vertex
		for (size_t i = 0; i < *faceVertecesCount; ++i)
		{
			if (almostEqual(faceVerteces[i], vertex, equalityTolerance))
			{
				return;
			}
		}
		// Check if the array is full
		if (*faceVertecesCount >= 4)
		{
			return;
		}
		faceVerteces[*faceVertecesCount] = vertex;
		*faceVertecesCount += 1;
	}

	size_t GeometryPrimitiveBase::getFaceVerteces(size_t vertexIndex, Vector3D normal, Vector3D* faceVerteces, GeometryTransform* geometryPrimitiveTransform, float equalityTolerance)
	{
		Vector3D closestNormal;
		Vector3D targetVertex = getVertexPosition(vertexIndex);
		float normalAngle = -mathem::max<float>();
		size_t trianglesCount = getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			TriangleGeometry3D resultTriangle = getTrianglePositions(i);
			if (resultTriangle.getPoint1() != targetVertex &&
				resultTriangle.getPoint2() != targetVertex &&
				resultTriangle.getPoint3() != targetVertex)
			{
				continue;
			}
			geometryPrimitiveTransform->applyTo(resultTriangle);
			Vector3D triangleNormal = resultTriangle.computeNormal();
			float newNormalAngle = (triangleNormal * normal);
			if (newNormalAngle > normalAngle)
			{
				normalAngle = newNormalAngle;
				closestNormal = triangleNormal;
				continue;
			}
		}

		size_t faceVertecesCount = 0;

		for (size_t i = 0; i < trianglesCount; ++i)
		{
			TriangleGeometry3D resultTriangle = getTrianglePositions(i);
			geometryPrimitiveTransform->applyTo(resultTriangle);
			Vector3D triangleNormal = resultTriangle.computeNormal();
			if (almostEqual(closestNormal, triangleNormal, equalityTolerance) == false)
			{
				continue;
			}

			tryAppendVertex(resultTriangle.getPoint1(), faceVerteces, &faceVertecesCount, equalityTolerance);
			tryAppendVertex(resultTriangle.getPoint2(), faceVerteces, &faceVertecesCount, equalityTolerance);
			tryAppendVertex(resultTriangle.getPoint3(), faceVerteces, &faceVertecesCount, equalityTolerance);

			if (faceVertecesCount >= 4)
			{
				break;
			}
		}
		return faceVertecesCount;
	}

	uint32_t GeometryPrimitiveBase::getVertecesCount()
	{
		switch (type)
		{
		case mathem::GeometryPrimitiveType::POINT:
			return 1;
		case mathem::GeometryPrimitiveType::LINE:
			return 2;
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return anyPrimitive.AABB.getVertecesCount();
		case mathem::GeometryPrimitiveType::BOX:
			return anyPrimitive.box.getVertecesCount();
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			return anyPrimitive.truncatedPyramid.getVertecesCount();
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			return anyPrimitive.convexHull->getVertecesCount();
		case mathem::GeometryPrimitiveType::NONE:
		case mathem::GeometryPrimitiveType::RAY:
		case mathem::GeometryPrimitiveType::PLANE:
		case mathem::GeometryPrimitiveType::SPHERE:
		case mathem::GeometryPrimitiveType::CAPSULE:
		default:
			break;
		}
		return 0;
	}

	Vector3D GeometryPrimitiveBase::getVertexPosition(uint32_t index)
	{
		switch (type)
		{
		case mathem::GeometryPrimitiveType::POINT:
			return anyPrimitive.point.position;
		case mathem::GeometryPrimitiveType::LINE:
			return anyPrimitive.line.points[index];
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return anyPrimitive.AABB.getVertexPosition(index);
		case mathem::GeometryPrimitiveType::BOX:
			return anyPrimitive.box.getVertexPosition(index);
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			return anyPrimitive.truncatedPyramid.getVertexPosition(index);
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			return anyPrimitive.convexHull->getVertexPosition(index);
		case mathem::GeometryPrimitiveType::NONE:
		case mathem::GeometryPrimitiveType::RAY:
		case mathem::GeometryPrimitiveType::PLANE:
		case mathem::GeometryPrimitiveType::SPHERE:
		case mathem::GeometryPrimitiveType::CAPSULE:
		default:
			break;
		}
		return Vector3D::zero();
	}

	uint32_t GeometryPrimitiveBase::getTrianglesCount()
	{
		if (isTriangularMesh() == false)
		{
			return 0;
		}

		switch (type)
		{
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return anyPrimitive.AABB.getTrianglesCount();
		case mathem::GeometryPrimitiveType::BOX:
			return anyPrimitive.box.getTrianglesCount();
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			return anyPrimitive.truncatedPyramid.getTrianglesCount();
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			return anyPrimitive.convexHull->getTrianglesCount();
		default:
			break;
		}
		return 0;
	}

	TriangleGeometry3D GeometryPrimitiveBase::getTrianglePositions(uint32_t index)
	{
		if (isTriangularMesh() == false)
		{
			return TriangleGeometry3D();
		}

		switch (type)
		{
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return anyPrimitive.AABB.getTrianglePositions(index);
		case mathem::GeometryPrimitiveType::BOX:
			return anyPrimitive.box.getTrianglePositions(index);
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			return anyPrimitive.truncatedPyramid.getTrianglePositions(index);
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			return anyPrimitive.convexHull->getTrianglePositions(index);
		default:
			break;
		}
		return TriangleGeometry3D();
	}
}