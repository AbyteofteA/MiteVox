
#ifndef PHYSICS_MOVEMENTPROPS_H
#define PHYSICS_MOVEMENTPROPS_H

#include "engine/Math/src/Math.h"

namespace physcs
{
	struct MovementProps
	{
		mathem::Vector3D pos = { 0 };
		mathem::Vector3D velocity = { 0 };
		mathem::Vector3D acceler = { 0 };

		mathem::Vector3D angle = { 0 };
		mathem::Vector3D angularVelocity = { 0 };
		mathem::Vector3D angularAcceler = { 0 };
	};
}

#endif