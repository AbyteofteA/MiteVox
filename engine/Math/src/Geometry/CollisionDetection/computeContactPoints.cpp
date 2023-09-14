#include "computeContactPoints.h"

#include "CollisionInfo.h"

namespace mathem
{
	void computeContactPoints(
		GeometryPrimitiveBase* geometryPrimitive1,
		GeometryTransform* geometryPrimitive1Transform,
		GeometryPrimitiveBase* geometryPrimitive2,
		GeometryTransform* geometryPrimitive2Transform,
		CollisionProperties* collisionProperties,
		float equalityTolerance)
	{
		switch (geometryPrimitive1->getType())
		{
		case mathem::GeometryPrimitiveType::POINT:
		{
			//Vector3D point = geometryPrimitive1Transform->translation; // TODO: is scale needed?
			// point.x() *= geometryPrimitive1Transform->scale.x();
			// point.y() *= geometryPrimitive1Transform->scale.y();
			// point.z() *= geometryPrimitive1Transform->scale.z();
			//computeContactPoints(&point, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);
			break;
		}
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
		case mathem::GeometryPrimitiveType::BOX:
		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
		case mathem::GeometryPrimitiveType::CONVEX_HULL:
			return computeContactPointsWithMesh(geometryPrimitive1, geometryPrimitive1Transform, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties, equalityTolerance);

		case mathem::GeometryPrimitiveType::SPHERE:
			// TODO: computeContactPoints((SphereGeometry*)geometryPrimitive1, geometryPrimitive1Transform, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);

		case mathem::GeometryPrimitiveType::CAPSULE:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::RAY:
			// TODO:
			break;

		default:
			break;
		}
	}
}