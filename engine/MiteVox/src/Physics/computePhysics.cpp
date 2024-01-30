#include "computePhysics.h"

#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Utilities/Convertations.h"
#include "engine/Math/src/Geometry/CollisionDetection/computeContactPoints.h"

#include <algorithm>

namespace mitevox
{
	void applyDampingAndSleeping(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime)
	{
		size_t entitiesToSimulateCount = entitiesToSimulate->getElementsCount();
		for (size_t i = 0; i < entitiesToSimulateCount; ++i)
		{
			Entity* entity = entitiesToSimulate->getElement(i);
			entity->applyDamping(substepDeltaTime);
			entity->updateSleeping(substepDeltaTime);
		}
	}

	void computeIntegration(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime)
	{
		size_t entitiesToSimulateCount = entitiesToSimulate->getElementsCount();

		for (size_t i = 0; i < entitiesToSimulateCount; ++i)
		{
			Entity* entity = entitiesToSimulate->getElement(i);

			entity->movementProperties.previousPosition = entity->transform.getPosition();
			entity->movementProperties.previousOrientation = entity->transform.getOrientation();
			if (entity->movementProperties.inverseMass != 0.0f)
			{
				mathem::Vector3D gravity = MiteVoxAPI::getGravity(entity);
				entity->integrateGravity(gravity, substepDeltaTime);
				entity->integrateForces(substepDeltaTime);
				entity->integrateVelocities(substepDeltaTime);
			}
		}
	}

	void resolveCollisionNoPhysics(float equalityTolerance);
	void computePhysicsNewtonian(safety::SafeArray<Entity*>* entitiesToSimulate, float deltaTime, float equalityTolerance);
	void computePositionBasedDynamics(safety::SafeArray<Entity*>* entitiesToSimulate, float deltaTime, float equalityTolerance);

	void computePhysics(PhysicsSolverType physicsSolverType, float deltaTime, float equalityTolerance)
	{
		safety::SafeArray<Entity*>* entitiesToSimulate = MiteVoxAPI::collectEntitiesToSimulate();

		switch (physicsSolverType)
		{
		case PhysicsSolverType::NO_PHYSICS:
			resolveCollisionNoPhysics(equalityTolerance);
			break;
		case PhysicsSolverType::NEWTONIAN:
			computePhysicsNewtonian(entitiesToSimulate, deltaTime, equalityTolerance);
			break;
		case mitevox::PhysicsSolverType::POSITION_BASED_LINEAR:
			// TODO:
			break;
		case mitevox::PhysicsSolverType::POSITION_BASED:
			computePositionBasedDynamics(entitiesToSimulate, deltaTime, equalityTolerance);
			break;
		case PhysicsSolverType::IMPULSE_BASED_LINEAR:
			// TODO:
			break;
		case PhysicsSolverType::IMPULSE_BASED:
			// TODO:
			break;
		default:
			break;
		}
	}
}