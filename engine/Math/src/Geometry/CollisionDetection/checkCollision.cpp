#include "checkCollision.h"

#include "CollisionInfo.h"
#include "computeContactPoints.h"

namespace mathem
{
	CollisionType checkCollision(
		GeometryPrimitiveBase* geometryPrimitive1,
		GeometryTransform* geometryPrimitive1Transform,
		GeometryPrimitiveBase* geometryPrimitive2,
		GeometryTransform* geometryPrimitive2Transform,
		CollisionProperties* collisionProperties,
		float equalityTolerance)
	{
		CollisionType resultCollisionType = CollisionType::NONE;
		collisionProperties->type = resultCollisionType;

		switch (geometryPrimitive1->getType())
		{
		case mathem::GeometryPrimitiveType::POINT:
		{
			Vector3D point = geometryPrimitive1Transform->translation; // TODO: is scale needed?
			// point.x() *= geometryPrimitive1Transform->scale.x();
			// point.y() *= geometryPrimitive1Transform->scale.y();
			// point.z() *= geometryPrimitive1Transform->scale.z();
			resultCollisionType = checkCollision(&point, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);
			break;
		}
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			resultCollisionType = checkCollision(
				(AxisAlignedBoxGeometry*)geometryPrimitive1, geometryPrimitive1Transform, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);
			break;

		case mathem::GeometryPrimitiveType::BOX:
			resultCollisionType = checkCollision(
				(BoxGeometry*)geometryPrimitive1, geometryPrimitive1Transform, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);
			break;

		case mathem::GeometryPrimitiveType::SPHERE:
			resultCollisionType = checkCollision(
				(SphereGeometry*)geometryPrimitive1, geometryPrimitive1Transform, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);
			break;

		case mathem::GeometryPrimitiveType::CAPSULE:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::RAY:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::MESH:
			// TODO:
			break;

		default:
			break;
		}
		
		return collisionProperties->type;
	}
}