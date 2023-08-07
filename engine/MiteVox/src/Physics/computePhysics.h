#ifndef COMPUTEPHYSICS_H
#define COMPUTEPHYSICS_H

#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace mitevox
{
	enum struct PhysicsSolverType
	{
		NO_PHYSICS,
		NEWTONIAN,
		POSITION_BASED_LINEAR,
		POSITION_BASED,
		IMPULSE_BASED_LINEAR,
		IMPULSE_BASED,
	};

	void computePhysics(PhysicsSolverType physicsSolverType, float deltaTime, float equalityTolerance);
	void computeIntegration(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime);
}

#endif