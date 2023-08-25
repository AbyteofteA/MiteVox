#ifndef CHECKCOLLISIONWITHPOINT_H
#define CHECKCOLLISIONWITHPOINT_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

namespace mathem
{
	CollisionType checkCollisionWithPoint(
		Vector3D point,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform);

	CollisionType checkCollisionWithPoint(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionPointVsBox(
		Vector3D point,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform);

	CollisionType checkCollisionPointVsBox(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionPointVsAABB(
		Vector3D point,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform);

	CollisionType checkCollisionPointVsAABB(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionPointVsSphere(
		Vector3D point,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform);

	CollisionType checkCollisionPointVsSphere(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionPointVsCapsule(
		Vector3D point,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform);

	CollisionType checkCollisionPointVsCapsule(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties);
}

#endif
