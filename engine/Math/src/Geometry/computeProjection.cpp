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
		Vector3D tmpPoint = box->getPoint(0, boxTransform);
		*max = dotProduct(tmpPoint, *vector);
		*min = *max;
		for (uint16_t pointIndex = 1; pointIndex < 8; ++pointIndex)
		{
			tmpPoint = box->getPoint(pointIndex, boxTransform);
			float tmpProjection = dotProduct(tmpPoint, *vector);

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
		Vector3D spherePosition = sphere->position;
		// TODO: transform spherePosition via sphereTransform
		float spherePositionProjection = dotProduct(spherePosition, *vector);
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
		Vector3D capsulePosition1 = capsule->position;
		capsulePosition1.j += capsule->halfHeight;
		Vector3D capsulePosition2 = capsule->position;
		capsulePosition2.j -= capsule->halfHeight;
		// TODO: transform capsulePosition1 via capsuleTransform
		// TODO: transform capsulePosition2 via capsuleTransform
		float spherePositionProjectionMin = dotProduct(capsulePosition1, *vector);
		float spherePositionProjectionMax = dotProduct(capsulePosition2, *vector);
		if (spherePositionProjectionMax < spherePositionProjectionMin)
		{
			std::swap(spherePositionProjectionMin, spherePositionProjectionMax);
		}
		*min = spherePositionProjectionMin - capsule->radius;
		*max = spherePositionProjectionMax + capsule->radius;
	}
}