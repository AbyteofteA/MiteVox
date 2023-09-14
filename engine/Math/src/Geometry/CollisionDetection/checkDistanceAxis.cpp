#include "checkDistanceAxis.h"

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeProjection.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computePenetration.h"

namespace mathem
{
	CollisionType checkDistanceAxis(
		GeometryPrimitiveBase* object1,
		GeometryTransform* objectTransform1,
		GeometryPrimitiveBase* object2,
		GeometryTransform* objectTransform2)
	{
		Vector3D position1 = Vector3D::zero();
		objectTransform1->applyTo(position1);

		Vector3D position2 = Vector3D::zero();
		objectTransform2->applyTo(position2);

		float object1ProjectionMin = 0.0f, object1ProjectionMax = 0.0f;
		float object2ProjectionMin = 0.0f, object2ProjectionMax = 0.0f;

		Vector3D normal = position2 - position1;
		normal.normalize();

		computeProjection(object1, objectTransform1, &normal, &object1ProjectionMin, &object1ProjectionMax);
		computeProjection(object2, objectTransform2, &normal, &object2ProjectionMin, &object2ProjectionMax);
		float penetration = computePenetration(object1ProjectionMin, object1ProjectionMax, object2ProjectionMin, object2ProjectionMax);
		if (penetration == 0.0f)
		{
			return CollisionType::NONE;
		}
		return CollisionType::INTERSECTION;
	}
}