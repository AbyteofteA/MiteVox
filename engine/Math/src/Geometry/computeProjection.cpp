#include "computeProjection.h"

#include <utility>

namespace mathem
{
	void computeProjection(
		BoxGeometry* box, 
		GeometryTransform* boxTransform, 
		Vector3D* vector, 
		float* min, 
		float* max)
	{
		Vector3D tmpPoint = box->getVertexPosition(0);
		boxTransform->applyTo(tmpPoint);
		*max = tmpPoint * (*vector);
		*min = *max;
		for (uint16_t pointIndex = 1; pointIndex < 8; ++pointIndex)
		{
			tmpPoint = box->getVertexPosition(pointIndex);
			boxTransform->applyTo(tmpPoint);
			float tmpProjection = tmpPoint * (*vector);

			if (tmpProjection < *min)
			{
				*min = tmpProjection;
			}
			if (tmpProjection > *max)
			{
				*max = tmpProjection;
			}
		}
	}

	void computeProjection(
		SphereGeometry* sphere, 
		GeometryTransform* sphereTransform, 
		Vector3D* vector, 
		float* min, 
		float* max)
	{
		Vector3D spherePosition = sphere->translation;
		// TODO: transform spherePosition via sphereTransform
		float spherePositionProjection = spherePosition * (*vector);
		*min = spherePositionProjection - sphere->radius;
		*max = spherePositionProjection + sphere->radius;
	}

	void computeProjection(
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		Vector3D* vector,
		float* min,
		float* max)
	{
		Vector3D capsulePosition1 = capsule->translation;
		capsulePosition1.y() += capsule->halfHeight;
		Vector3D capsulePosition2 = capsule->translation;
		capsulePosition2.y() -= capsule->halfHeight;
		capsuleTransform->applyTo(capsulePosition1);
		capsuleTransform->applyTo(capsulePosition2);

		float spherePositionProjectionMin = capsulePosition1 * (*vector);
		float spherePositionProjectionMax = capsulePosition2 * (*vector);
		if (spherePositionProjectionMax < spherePositionProjectionMin)
		{
			std::swap(spherePositionProjectionMin, spherePositionProjectionMax);
		}
		*min = spherePositionProjectionMin - capsule->radius;
		*max = spherePositionProjectionMax + capsule->radius;
	}
}