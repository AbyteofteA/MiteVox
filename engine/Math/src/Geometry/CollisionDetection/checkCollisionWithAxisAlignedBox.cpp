#include "checkCollisionWithAxisAlignedBox.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollisionWithBox.h"

namespace mathem
{
	CollisionType checkCollisionAABBvsBox(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties)
	{
		CollisionType result = checkCollisionBoxVsAABB(box, boxTransform, axisAlignedBox, axisAlignedBoxTransform, collisionProperties);
		collisionProperties->normalBelongsToTheFirst = !collisionProperties->normalBelongsToTheFirst;
		return result;
	}

	CollisionType checkCollisionAABBvsAABB(
		GeometryPrimitiveBase* axisAlignedBox1,
		GeometryTransform* axisAlignedBox1Transform,
		GeometryPrimitiveBase* axisAlignedBox2,
		GeometryTransform* axisAlignedBox2Transform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box1 = axisAlignedBox1->axisAlignedBoxToBox();
		GeometryPrimitiveBase box2 = axisAlignedBox2->axisAlignedBoxToBox();
		return checkCollisionBoxVsBox(&box1, axisAlignedBox1Transform, &box2, axisAlignedBox2Transform, collisionProperties);
	}

	CollisionType checkCollisionAABBvsSphere(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box = axisAlignedBox->axisAlignedBoxToBox();
		return checkCollisionBoxVsSphere(&box, axisAlignedBoxTransform, sphere, sphereTransform, collisionProperties);
	}

	CollisionType checkCollisionAABBvsCapsule(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box = axisAlignedBox->axisAlignedBoxToBox();
		return checkCollisionBoxVsCapsule(&box, axisAlignedBoxTransform, capsule, capsuleTransform, collisionProperties);
	}

	CollisionType checkCollisionAABBvsTruncatedPyramid(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box = axisAlignedBox->axisAlignedBoxToBox();
		return checkCollisionBoxVsTruncatedPyramid(&box, axisAlignedBoxTransform, truncatedPyramid, truncatedPyramidTransform, collisionProperties);
	}
}