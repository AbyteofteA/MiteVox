#ifndef CHECKCOLLISIONWITHBOX_H
#define CHECKCOLLISIONWITHBOX_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

namespace mathem
{
	/// checkCollision - BOX vs BOX
	/// 
	/// Algorithm:
	/// 1. Get (transformed) triangles 0, 2, 4 from box1
	/// 2. Extract normals from the triangles
	/// 3. Compute the box1's & box2's projections onto the normals (via dot product)
	/// 4. Check if the projections overlap
	/// 5. Repeat the algorithm for the box2
	CollisionType checkCollisionBoxVsBox(
		GeometryPrimitiveBase* box1,
		GeometryTransform* box1Transform,
		GeometryPrimitiveBase* box2,
		GeometryTransform* box2Transform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionBoxVsAABB(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionBoxVsSphere(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionBoxVsCapsule(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionBoxVsTruncatedPyramid(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties);
}

#endif