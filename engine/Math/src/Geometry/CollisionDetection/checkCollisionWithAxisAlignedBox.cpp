#include "checkCollision.h"

#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	bool checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox1,
		AxisAlignedBoxGeometry* axisAlignedBox2,
		CollisionInfo* collisionInfo)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryTransform* box1Transform = new GeometryTransform();
		BoxGeometry* box1 = new BoxGeometry();
		box1->halfSize = axisAlignedBox1->halfSize;
		box1->transform.translation = axisAlignedBox1->position;

		GeometryTransform* box2Transform = new GeometryTransform();
		BoxGeometry* box2 = new BoxGeometry();
		box2->halfSize = axisAlignedBox2->halfSize;
		box2->transform.translation = axisAlignedBox2->position;

		bool result = checkCollision(box1, box2Transform, box2, box2Transform, collisionInfo);
		delete box1Transform;
		delete box1;
		delete box2Transform;
		delete box2;
		return result;
	}

	bool checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionInfo* collisionInfo)
	{
		Vector3D collisionPoint;

		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
		{
			BoxGeometry* otherBox = (BoxGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			return checkCollision(otherBox, otherGeometryTransform, axisAlignedBox, collisionInfo);
		}

		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
		{
			AxisAlignedBoxGeometry* otherBox = (AxisAlignedBoxGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			return checkCollision(axisAlignedBox, otherBox, collisionInfo);
		}

		case GeometryPrimitiveType::SPHERE:
		{
			SphereGeometry* otherSphere = (SphereGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			// TODO: return checkCollision(axisAlignedBox, otherSphere, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::CAPSULE:
		{
			CapsuleGeometry* otherCapsule = (CapsuleGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			// TODO: return checkCollision(axisAlignedBox, otherCapsule, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
		{
			TruncatedPyramidGeometry* otherTruncatedPyramid = (TruncatedPyramidGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			// TODO: return checkCollision(axisAlignedBox, otherTruncatedPyramid, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::MESH:
		{
			mitevox::Mesh* otherMesh = (mitevox::Mesh*)otherGeometry;
			Vector3D tmpCollisionPoint;
			// TODO: return checkCollision(axisAlignedBox, otherMesh, otherGeometryTransform, collisionInfo);
		}

		default:
			break;
		}
		return false;
	}
}