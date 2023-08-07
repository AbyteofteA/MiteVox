#include "engine/MiteVox/src/Physics/computePhysics.h"

#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"
#include "engine/Math/src/Vector.h"

namespace mitevox
{
	void computeIntegration(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime)
	{
		size_t entitiesToSimulateCount = entitiesToSimulate->getElementsCount();

		for (size_t i = 0; i < entitiesToSimulateCount; ++i)
		{
			Entity* entity = entitiesToSimulate->getElement(i);

			entity->movementProperties.previousPosition = entity->transform.translation;
			entity->movementProperties.previousOrientation = entity->transform.rotation;
			if (entity->movementProperties.inverseMass != 0.0f)
			{
				mathem::Vector3D gravity = MiteVoxAPI::getGravity(entity);
				entity->integrateGravity(gravity, substepDeltaTime);
				entity->integrateForces(substepDeltaTime);
				entity->integrateVelocities(substepDeltaTime);
			}
		}
	}
}