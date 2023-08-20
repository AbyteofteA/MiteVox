#ifndef COMPUTEPHYSICS_H
#define COMPUTEPHYSICS_H

#include "engine/MiteVox/src/Physics/PhysicsSolverType.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace mitevox
{
	void applyDampingAndSleeping(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime);
	void computeIntegration(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime);
	void computePhysics(PhysicsSolverType physicsSolverType, float deltaTime, float equalityTolerance);
}

#endif