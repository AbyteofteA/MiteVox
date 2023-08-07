#ifndef PHYSICS_MOVEMENTPROPERTIES_H
#define PHYSICS_MOVEMENTPROPERTIES_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Quaternion.h"

namespace mitevox
{
	/// <summary>
	/// Stores secondary (computed) movement parameters
	/// </summary>
	struct MovementProperties
	{
		float inverseMass = 0.0f;
		mathem::Vector3D velocity = { 0.0f, 0.0f, 0.0f };
		mathem::Vector3D angularVelocity = { 0.0f, 0.0f, 0.0f };

		mathem::Vector3D previousPosition = { 0.0f, 0.0f, 0.0f };
		mathem::Vector3D previousVelocity = { 0.0f, 0.0f, 0.0f };

		mathem::Quaternion previousOrientation;
		mathem::Vector3D previousAngularVelocity = { 0.0f, 0.0f, 0.0f };

		mathem::Vector3D externalForces = { 0.0f, 0.0f, 0.0f };
		mathem::Vector3D externalTorque = { 0.0f, 0.0f, 0.0f };

		mathem::Matrix3x3 momentOfInertia;
		mathem::Matrix3x3 inverseMomentOfInertia;
	};
}

#endif