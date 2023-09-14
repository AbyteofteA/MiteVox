#ifndef CHECKCOLLISIONWITHAXISALIGNEDBOX_H
#define CHECKCOLLISIONWITHAXISALIGNEDBOX_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

namespace mathem
{
	CollisionType checkCollisionAABBvsBox(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionAABBvsAABB(
		GeometryPrimitiveBase* axisAlignedBox1,
		GeometryTransform* axisAlignedBox1Transform,
		GeometryPrimitiveBase* axisAlignedBox2,
		GeometryTransform* axisAlignedBox2Transform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionAABBvsSphere(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionAABBvsCapsule(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionAABBvsTruncatedPyramid(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionAABBvsConvexHull(
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		CollisionProperties* collisionProperties);
}

#endif