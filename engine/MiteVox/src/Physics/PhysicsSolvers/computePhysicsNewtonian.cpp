#include "engine/MiteVox/src/Physics/computePhysics.h"

#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/computeContactPoints.h"

namespace mitevox
{
	void separateObjects(mathem::CollisionInfo<Entity*>* collisionInfo);
	void resolveCollisionNewtonian(mathem::CollisionInfo<Entity*>* collisionInfo, float deltaTime, float equalityTolerance);

	void computePhysicsNewtonian(safety::SafeArray<Entity*>* entitiesToSimulate, float deltaTime, float equalityTolerance)
	{
		safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions = MiteVoxAPI::computeCollisions();

		size_t collisionsCount = collisions->getElementsCount();
		for (size_t i = 0; i < collisionsCount; ++i)
		{
			mathem::CollisionInfo<Entity*> collisionInfo = collisions->getElement(i);
			separateObjects(&collisionInfo);
			computeContactPoints(&collisionInfo, MiteVoxAPI::getSettings()->getEqualityTolerance()); // TODO: delete
			resolveCollisionNewtonian(&collisionInfo, deltaTime, equalityTolerance);
			collisions->setElement(i, collisionInfo);
		}
	}

	void separateObjects(mathem::CollisionInfo<Entity*>* collisionInfo)
	{
		mathem::GeometryTransform* objectTransform1 = collisionInfo->object1->getResultTransform();
		mathem::GeometryTransform* objectTransform2 = collisionInfo->object2->getResultTransform();
		MovementProperties* movementProperties1 = &collisionInfo->object1->movementProperties;
		MovementProperties* movementProperties2 = &collisionInfo->object2->movementProperties;

		mathem::Vector3D separationVector = 
			-collisionInfo->properties.normal * collisionInfo->properties.penetrationDepth;
		mathem::Vector3D positionalImpulse = separationVector / (movementProperties1->inverseMass + movementProperties2->inverseMass);
		if (movementProperties1->inverseMass != 0.0f)
		{
			mathem::Vector3D separationVector1 = positionalImpulse * movementProperties1->inverseMass;
			objectTransform1->translation += separationVector1;
		}
		if (movementProperties2->inverseMass != 0.0f)
		{
			mathem::Vector3D separationVector2 = positionalImpulse * movementProperties2->inverseMass;
			objectTransform2->translation -= separationVector2;
		}
	}

	void resolveCollisionNewtonian(mathem::CollisionInfo<Entity*>* collisionInfo, float deltaTime, float equalityTolerance)
	{
		mathem::GeometryTransform* objectTransform1 = collisionInfo->object1->getResultTransform();
		mathem::GeometryTransform* objectTransform2 = collisionInfo->object2->getResultTransform();
		MovementProperties* movementProperties1 = &collisionInfo->object1->movementProperties;
		MovementProperties* movementProperties2 = &collisionInfo->object2->movementProperties;
		PhysicalMaterial* physicalMaterial1 = &collisionInfo->object1->physicalMaterial;
		PhysicalMaterial* physicalMaterial2 = &collisionInfo->object2->physicalMaterial;

		mathem::Vector3D relativeVelocity = movementProperties1->velocity - movementProperties2->velocity;
		if (relativeVelocity * collisionInfo->properties.normal < 0.0f)
		{
			return;
		}

		float mass1 = collisionInfo->object1->getMass();
		float mass2 = collisionInfo->object2->getMass();
		float overallMass = mass1 + mass2;
		if (overallMass == 0.0f)
		{
			return;
		}

		float averageRestitution = (physicalMaterial1->getRestitution() + physicalMaterial2->getRestitution()) * 0.5f;
		if (mass1 == 0.0f)
		{
			float velocity2 = movementProperties2->velocity * collisionInfo->properties.normal;
			movementProperties2->velocity -= collisionInfo->properties.normal * velocity2 * (1.0f + averageRestitution);
			return;
		}
		if (mass2 == 0.0f)
		{
			float velocity1 = movementProperties1->velocity * collisionInfo->properties.normal;
			movementProperties1->velocity -= collisionInfo->properties.normal * velocity1 * (1.0f + averageRestitution);
			return;
		}

		float velocity1 = movementProperties1->velocity * collisionInfo->properties.normal;
		float velocity2 = movementProperties2->velocity * collisionInfo->properties.normal;
		float impulse = -relativeVelocity * collisionInfo->properties.normal * (mass1 * mass2 / overallMass) * (1.0f + averageRestitution);
		float velocityDelta1 = impulse * movementProperties1->inverseMass;
		float velocityDelta2 = impulse * movementProperties2->inverseMass;
		movementProperties1->velocity += collisionInfo->properties.normal * velocityDelta1;
		movementProperties2->velocity -= collisionInfo->properties.normal * velocityDelta2;
	}
}