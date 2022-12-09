#include "computeProjection.h"

#include <utility>

namespace mathem
{
	void computeProjection(
		GeometryPrimitiveBase* triangularMesh,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max)
	{
		Vector3D boxPoint = triangularMesh->getVertexPosition(0);
		parentTransform->applyTo(boxPoint);
		*max = boxPoint * (*vector); // Compute projection of the first point
		*min = *max;
		for (uint16_t pointIndex = 1; pointIndex < 8; ++pointIndex)
		{
			boxPoint = triangularMesh->getVertexPosition(pointIndex);
			parentTransform->applyTo(boxPoint);
			float pointProjection = boxPoint * (*vector);

			if (pointProjection < *min)
			{
				*min = pointProjection;
			}
			if (pointProjection > *max)
			{
				*max = pointProjection;
			}
		}
	}

	void computeProjection(
		SphereGeometry* sphere, 
		GeometryTransform* parentTransform,
		Vector3D* vector, 
		float* min, 
		float* max)
	{
		Vector3D spherePosition = sphere->transform.translation;
		parentTransform->applyTo(spherePosition);
		float spherePositionProjection = spherePosition * (*vector);
		float actualSphereRadius = sphere->radius * sphere->transform.scale.x() * parentTransform->scale.x();
		*min = spherePositionProjection - actualSphereRadius;
		*max = spherePositionProjection + actualSphereRadius;
	}

	void computeProjection(
		CapsuleGeometry* capsule,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max)
	{
		float actualCapsuleHalfHeight = capsule->halfHeight * capsule->transform.scale.x();
		Vector3D capsulePosition1 = capsule->transform.translation;
		Vector3D capsulePosition2 = capsule->transform.translation;
		capsulePosition1.y() += actualCapsuleHalfHeight;
		capsulePosition2.y() -= actualCapsuleHalfHeight;
		parentTransform->applyTo(capsulePosition1);
		parentTransform->applyTo(capsulePosition2);

		float spherePositionProjectionMin = capsulePosition1 * (*vector);
		float spherePositionProjectionMax = capsulePosition2 * (*vector);
		if (spherePositionProjectionMax < spherePositionProjectionMin)
		{
			std::swap(spherePositionProjectionMin, spherePositionProjectionMax);
		}
		float actualCapsuleRadius = capsule->radius * capsule->transform.scale.x() * parentTransform->scale.x();
		*min = spherePositionProjectionMin - actualCapsuleRadius;
		*max = spherePositionProjectionMax + actualCapsuleRadius;
	}
}