#include "checkCollision.h"

#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"

namespace mathem
{
	CollisionType checkCollision(
		SphereGeometry* sphere1,
		GeometryTransform* sphere1Transform,
		SphereGeometry* sphere2,
		GeometryTransform* sphere2Transform,
		CollisionProperties* collisionProperties);

	// TODO: checkCollision SPHERE vs CAPSULE
	// TODO: checkCollision SPHERE vs TRUNCATED_PYRAMID
	// TODO: checkCollision SPHERE vs MESH
	// TODO: checkCollision SPHERE vs RAY

	CollisionType checkCollision(
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties)
	{
		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
		{
			CollisionType collisionType = checkCollision((BoxGeometry*)otherGeometry, otherGeometryTransform, sphere, sphereTransform, collisionProperties);
			collisionProperties->invert();
			return collisionType;
		}
		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
		{
			CollisionType collisionType = checkCollision((AxisAlignedBoxGeometry*)otherGeometry, otherGeometryTransform, sphere, sphereTransform, collisionProperties);
			collisionProperties->invert();
			return collisionType;
		}
		case GeometryPrimitiveType::SPHERE:
			return checkCollision(sphere, sphereTransform, (SphereGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::CAPSULE:
			// TODO: return checkCollision(sphere, sphereTransform, (CapsuleGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
			// TODO: return checkCollision(sphere, sphereTransform, (TruncatedPyramidGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::MESH:
			// TODO: return checkCollision(sphere, sphereTransform, (mitevox::Mesh*)otherGeometry, otherGeometryTransform, collisionInfo);

		default:
			break;
		}
		return CollisionType::NONE;
	}

	CollisionType checkCollision(
		SphereGeometry* sphere1, 
		GeometryTransform* sphere1Transform,
		SphereGeometry* sphere2,
		GeometryTransform* sphere2Transform,
		CollisionProperties* collisionProperties)
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
			return CollisionType::NONE;
		}

		collisionProperties->type = CollisionType::INTERSECTION;
		// TODO: compute collision properties
		return CollisionType::INTERSECTION;
	}
}
