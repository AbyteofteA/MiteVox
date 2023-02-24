#ifndef PHYSICS_MOVEMENTPROPERTIES_H
#define PHYSICS_MOVEMENTPROPERTIES_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Quaternion.h"

namespace physcs
{
	struct MovementProperties
	{
		float inverseMass = 0.0f;
		float inverseMomentOfInertia = 0.0f;
		float restitution = 1.0f;

		mathem::Vector3D previousPosition = { 0.0f, 0.0f, 0.0f };
		mathem::Quaternion previousOrientation;

		mathem::Vector3D velocity = { 0.0f, 0.0f, 0.0f };
		mathem::Vector3D velocityDelta = { 0.0f, 0.0f, 0.0f };

		mathem::Vector3D angularVelocity = { 0.0f, 0.0f, 0.0f };
		mathem::Vector3D angularVelocityDelta = { 0.0f, 0.0f, 0.0f };

		mathem::Vector3D force = { 0.0f, 0.0f, 0.0f };

		float getMass();
	};
}

#endif