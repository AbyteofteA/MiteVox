#include "computePhysics.h"

#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Convertations.h"
#include "engine/Math/src/Geometry/CollisionDetection/computeContactPoints.h"

#include <algorithm>

namespace mitevox
{
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