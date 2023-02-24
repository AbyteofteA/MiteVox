#include "checkCollision.h"

#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox1,
		GeometryTransform* axisAlignedBox1Transform,
		AxisAlignedBoxGeometry* axisAlignedBox2,
		GeometryTransform* axisAlignedBox2Transform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		TruncatedPyramidGeometry* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties);

	// TODO: checkCollision AXIS_ALIGNED_BOX vs MESH
	// TODO: checkCollision AXIS_ALIGNED_BOX vs RAY

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties)
	{
		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
		{
			CollisionType collisionType = checkCollision((BoxGeometry*)otherGeometry, otherGeometryTransform, axisAlignedBox, axisAlignedBoxTransform, collisionProperties);
			collisionProperties->invert();
			return collisionType;
		}
		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return checkCollision(axisAlignedBox, axisAlignedBoxTransform, (AxisAlignedBoxGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::SPHERE:
			return checkCollision(axisAlignedBox, axisAlignedBoxTransform, (SphereGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::CAPSULE:
			return checkCollision(axisAlignedBox, axisAlignedBoxTransform, (CapsuleGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
			return checkCollision(axisAlignedBox, axisAlignedBoxTransform, (TruncatedPyramidGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::MESH:
			// TODO: return checkCollision(axisAlignedBox, axisAlignedBoxTransform, (mitevox::Mesh*)otherGeometry, otherGeometryTransform, collisionInfo);

		default:
			break;
		}
		return CollisionType::NONE;
	}

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox1,
		GeometryTransform* axisAlignedBox1Transform,
		AxisAlignedBoxGeometry* axisAlignedBox2,
		GeometryTransform* axisAlignedBox2Transform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		BoxGeometry box1;
		box1.halfSize = axisAlignedBox1->halfSize;
		box1.transform.translation = axisAlignedBox1->position;

		BoxGeometry box2;
		box2.halfSize = axisAlignedBox2->halfSize;
		box2.transform.translation = axisAlignedBox2->position;

		return checkCollision(&box1, axisAlignedBox1Transform, &box2, axisAlignedBox2Transform, collisionProperties);
	}

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		BoxGeometry box;
		box.halfSize = axisAlignedBox->halfSize;
		box.transform.translation = axisAlignedBox->position;

		return checkCollision(&box, axisAlignedBoxTransform, sphere, sphereTransform, collisionProperties);
	}

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		BoxGeometry box;
		box.halfSize = axisAlignedBox->halfSize;
		box.transform.translation = axisAlignedBox->position;

		return checkCollision(&box, axisAlignedBoxTransform, capsule, capsuleTransform, collisionProperties);
	}

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		TruncatedPyramidGeometry* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		BoxGeometry box;
		box.halfSize = axisAlignedBox->halfSize;
		box.transform.translation = axisAlignedBox->position;

		return checkCollision(&box, axisAlignedBoxTransform, truncatedPyramid, truncatedPyramidTransform, collisionProperties);
	}
}