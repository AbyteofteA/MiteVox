#include "checkCollisionWithSphere.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollisionWithBox.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollisionWithAxisAlignedBox.h"

namespace mathem
{
	CollisionType checkCollisionSphereVsBox(
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties)
	{
		CollisionType result = checkCollisionBoxVsSphere(box, boxTransform, sphere, sphereTransform, collisionProperties);
		collisionProperties->normalBelongsToTheFirst = !collisionProperties->normalBelongsToTheFirst;
		return result;
	}

	CollisionType checkCollisionSphereVsAABB(
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		GeometryPrimitiveBase* AABB,
		GeometryTransform* AABBTransform,
		CollisionProperties* collisionProperties)
	{
		CollisionType result = checkCollisionBoxVsAABB(AABB, AABBTransform, sphere, sphereTransform, collisionProperties);
		collisionProperties->normalBelongsToTheFirst = !collisionProperties->normalBelongsToTheFirst;
		return result;
	}

	CollisionType checkCollisionSphereVsSphere(
		GeometryPrimitiveBase* sphere1,
		GeometryTransform* sphere1Transform,
		GeometryPrimitiveBase* sphere2,
		GeometryTransform* sphere2Transform,
		CollisionProperties* collisionProperties)
	{
		bool thereMayBeACollision = true;

		SphereGeometry* sphereGeometry1 = sphere1->getSphere();
		SphereGeometry* sphereGeometry2 = sphere2->getSphere();

		Vector3D sphere1Position = sphereGeometry1->transform.translation;
		sphere1Transform->applyTo(sphere1Position);
		float sphere1Radius = sphereGeometry1->radius * sphereGeometry1->transform.scale.x() * sphere1Transform->scale.x();

		Vector3D sphere2Position = sphereGeometry2->transform.translation;
		sphere2Transform->applyTo(sphere2Position);
		float sphere2Radius = sphereGeometry2->radius * sphereGeometry2->transform.scale.x() * sphere2Transform->scale.x();

		float minimumDistance = sphere1Radius + sphere2Radius;
		
		if (std::abs(sphere1Position.x() - sphere2Position.x()) > minimumDistance)
		{
			thereMayBeACollision = false;
		}
		if (std::abs(sphere1Position.y() - sphere2Position.y()) > minimumDistance)
		{
			thereMayBeACollision = false;
		}
		if (std::abs(sphere1Position.z() - sphere2Position.z()) > minimumDistance)
		{
			thereMayBeACollision = false;
		}

		if (thereMayBeACollision == false)
		{
			return CollisionType::NONE;
		}

		collisionProperties->type = CollisionType::INTERSECTION;
		// TODO: compute collision properties
		return CollisionType::INTERSECTION;
	}
}
