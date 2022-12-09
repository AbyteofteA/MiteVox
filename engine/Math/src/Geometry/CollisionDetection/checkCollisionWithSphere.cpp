#include "checkCollision.h"

#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"

namespace mathem
{
	bool checkCollision(
		SphereGeometry* sphere1, 
		GeometryTransform* sphere1Transform,
		SphereGeometry* sphere2,
		GeometryTransform* sphere2Transform,
		CollisionInfo* collisionInfo)
	{
		bool thereMayBeACollision = true;

		Vector3D sphere1Position = sphere1->transform.translation;
		sphere1Transform->applyTo(sphere1Position);
		float sphere1Radius = sphere1->radius * sphere1->transform.scale.x() * sphere1Transform->scale.x();

		Vector3D sphere2Position = sphere2->transform.translation;
		sphere2Transform->applyTo(sphere2Position);
		float sphere2Radius = sphere2->radius * sphere2->transform.scale.x() * sphere2Transform->scale.x();

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
			return false;
		}

		if (collisionInfo)
		{
			collisionInfo->object1 = sphere1;
			collisionInfo->object2 = sphere2;
			collisionInfo->type = CollisionType::INTERSECTION;
			// TODO: compute collision info
		}
		return true;
	}
}
