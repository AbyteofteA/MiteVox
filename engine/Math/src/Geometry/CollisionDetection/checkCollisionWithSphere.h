#ifndef CHECKCOLLISIONWITHSPHERE_H
#define CHECKCOLLISIONWITHSPHERE_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

namespace mathem
{
	CollisionType checkCollisionSphereVsBox(
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionSphereVsAABB(
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		GeometryPrimitiveBase* AABB,
		GeometryTransform* AABBTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionSphereVsSphere(
		GeometryPrimitiveBase* sphere1,
		GeometryTransform* sphere1Transform,
		GeometryPrimitiveBase* sphere2,
		GeometryTransform* sphere2Transform,
		CollisionProperties* collisionProperties);
}

#endif