#include "engine/MiteVox/src/Physics/computePhysics.h"

#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/Physics/PhysicsSolvers/separateObjects.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/computeContactPoints.h"

namespace mitevox
{
	void resolveCollisionNoPhysics(float equalityTolerance)
	{
		safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions = MiteVoxAPI::computeCollisions();

		size_t collisionsCount = collisions->getElementsCount();
		for (size_t i = 0; i < collisionsCount; ++i)
		{
			mathem::CollisionInfo<Entity*> collisionInfo = collisions->getElement(i);
			separateObjects(&collisionInfo);
			computeContactPoints(&collisionInfo, MiteVoxAPI::getSettings()->getEqualityTolerance()); // TODO: delete
			collisions->setElement(i, collisionInfo);
		}
	}
}