#include "engine/MiteVox/src/Physics/computePhysics.h"

#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/computeContactPoints.h"

#include <algorithm>

namespace mitevox
{
	void solveConstraints(safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions, float substepDeltaTime);
	void deriveVelocities(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime);
	void solveVelocities(safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions, float substepDeltaTime);

	/// <summary>
	/// A naive implementation of Extended Position Based Dynamics.
	/// Original paper: https://matthias-research.github.io/pages/publications/PBDBodies.pdf
	/// </summary>
	/// <param name="entitiesToSimulate"></param>
	/// <param name="substepDeltaTime"></param>
	/// <param name="equalityTolerance"></param>
	void computePositionBasedDynamics(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime, float equalityTolerance)
	{
		safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions = MiteVoxAPI::computeCollisions();
		solveConstraints(collisions, substepDeltaTime);
		deriveVelocities(entitiesToSimulate, substepDeltaTime);
		solveVelocities(collisions, substepDeltaTime);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>true if both objects are sleeping, otherwise false and awake both objects</returns>
	bool tryToSkipBecauseOfSleeping(mathem::CollisionInfo<Entity*>* collisionInfo)
	{
		Entity* object1 = collisionInfo->object1;
		Entity* object2 = collisionInfo->object2;
		if (object1->isSleeping() &&
			object2->isSleeping())
		{
			return true;
		}
		
		object1->awake(MiteVoxAPI::getCurrentDeltaTime());
		object2->awake(MiteVoxAPI::getCurrentDeltaTime());
		return false;
	}

	void solveConstraints(safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions, float substepDeltaTime)
	{
		size_t collisionsCount = collisions->getElementsCount();
		for (size_t i = 0; i < collisionsCount; ++i)
		{
			mathem::CollisionInfo<Entity*>* collisionInfo = collisions->getElementPointer(i);

			if (tryToSkipBecauseOfSleeping(collisionInfo))
			{
				continue;
			}

			computeContactPoints(collisionInfo, MiteVoxAPI::getSettings()->getEqualityTolerance());
			collisions->setElement(i, *collisionInfo);

			mathem::GeometryTransform* objectTransform1 = collisionInfo->object1->getResultTransform();
			mathem::GeometryTransform* objectTransform2 = collisionInfo->object2->getResultTransform();
			MovementProperties* movementProperties1 = &collisionInfo->object1->movementProperties;
			MovementProperties* movementProperties2 = &collisionInfo->object2->movementProperties;
			PhysicalMaterial* physicalMaterial1 = &collisionInfo->object1->physicalMaterial;
			PhysicalMaterial* physicalMaterial2 = &collisionInfo->object2->physicalMaterial;
			float staticFriction = (physicalMaterial1->getStaticFriction() + physicalMaterial2->getStaticFriction()) * 0.5f;

			float inverseMass1 = movementProperties1->inverseMass;
			float inverseMass2 = movementProperties2->inverseMass;

			for (size_t i = 0; i < collisionInfo->properties.contactPointsCount; ++i)
			{
				mathem::Matrix3x3 inverseI1 = collisionInfo->object1->getInverseMomentOfInertia();
				mathem::Matrix3x3 inverseI2 = collisionInfo->object2->getInverseMomentOfInertia();

				mathem::Vector3D r1 = collisionInfo->properties.contacts[i].contactPoints1;
				r1 = objectTransform1->rotation.rotate(r1);
				mathem::Vector3D r2 = collisionInfo->properties.contacts[i].contactPoints2;
				r2 = objectTransform2->rotation.rotate(r2);
				mathem::Vector3D contactPoints1GlobalCoords = r1 + objectTransform1->translation;
				mathem::Vector3D contactPoints2GlobalCoords = r2 + objectTransform2->translation;

				mathem::Vector3D deltaX = contactPoints1GlobalCoords - contactPoints2GlobalCoords;
				mathem::Vector3D n = collisionInfo->properties.normal;
				float penetrationDepth = deltaX * n;

				if (penetrationDepth <= 0.0f)
				{
					continue;
				}

				mathem::Vector3D r1Prev = collisionInfo->properties.contacts[i].contactPoints1;
				r1Prev = movementProperties1->previousOrientation.rotate(r1Prev);
				mathem::Vector3D r2Prev = collisionInfo->properties.contacts[i].contactPoints2;
				r2Prev = movementProperties2->previousOrientation.rotate(r2Prev);
				mathem::Vector3D contactPoints1PrevGlobalCoords = r1Prev + movementProperties1->previousPosition;
				mathem::Vector3D contactPoints2PrevGlobalCoords = r2Prev + movementProperties2->previousPosition;

				mathem::Vector3D deltaP =
					(contactPoints1GlobalCoords - contactPoints1PrevGlobalCoords) -
					(contactPoints2GlobalCoords - contactPoints2PrevGlobalCoords);
				float deltaPNormal = deltaP * n;
				mathem::Vector3D deltaPTangent = deltaP - n * deltaPNormal;
				mathem::Vector3D tangent = deltaPTangent;
				tangent.normalize();

				mathem::Vector3D r1_x_n = mathem::crossProduct(r1, n);
				float generalizedInverseMass1 = movementProperties1->inverseMass + r1_x_n * multiply(inverseI1, r1_x_n);
				mathem::Vector3D r2_x_n = mathem::crossProduct(r2, n);
				float generalizedInverseMass2 = movementProperties2->inverseMass + r2_x_n * multiply(inverseI2, r2_x_n);
				float generalizedMass = 1.0f / (generalizedInverseMass1 + generalizedInverseMass2);

				float deltaLambda = -penetrationDepth * generalizedMass;
				collisionInfo->properties.contacts[i].normalForce = deltaLambda;

				float deltaLambdaT = -deltaPTangent.getLength() * generalizedMass;
				collisionInfo->properties.contacts[i].tangentForce = deltaLambdaT;

				// Static friction
				// TODO: seems like it's not correct, but looks decent
				mathem::Vector3D staticFrictionPositionalImpulse = { 0.0f, 0.0f, 0.0f };
				if (collisionInfo->properties.contacts[i].tangentForce <
					collisionInfo->properties.contacts[i].normalForce * staticFriction)
				{
					staticFrictionPositionalImpulse = -deltaPTangent * generalizedMass * 0.5f;
				}

				mathem::Vector3D positionalImpulse = n * deltaLambda + staticFrictionPositionalImpulse;

				if (inverseMass1 != 0.0f)
				{
					objectTransform1->translation += positionalImpulse * inverseMass1;
					mathem::Vector3D rotation1 = multiply(inverseI1, mathem::crossProduct(r1, positionalImpulse));
					mathem::Quaternion deltaOrientation1(
						rotation1.x(),
						rotation1.y(),
						rotation1.z(),
						0.0f);
					mathem::Quaternion worldDeltaOrientation1 = deltaOrientation1.multiplyCopy(objectTransform1->rotation);
					objectTransform1->rotation.components += worldDeltaOrientation1.components * 0.5f;
					objectTransform1->rotation.normalize();
				}

				if (inverseMass2 != 0.0f)
				{
					objectTransform2->translation -= positionalImpulse * inverseMass2;
					mathem::Vector3D rotation2 = multiply(inverseI2, mathem::crossProduct(r2, positionalImpulse));
					mathem::Quaternion deltaOrientation2(
						rotation2.x(),
						rotation2.y(),
						rotation2.z(),
						0.0f);
					mathem::Quaternion worldDeltaOrientation2 = deltaOrientation2.multiplyCopy(objectTransform2->rotation);
					objectTransform2->rotation.components -= worldDeltaOrientation2.components * 0.5f;
					objectTransform2->rotation.normalize();
				}
			}
		}
	}

	void deriveVelocities(safety::SafeArray<Entity*>* entitiesToSimulate, float substepDeltaTime)
	{
		float inverseSubstepDeltaTime = 1.0f / substepDeltaTime;

		size_t entitiesToSimulateCount = entitiesToSimulate->getElementsCount();
		for (size_t i = 0; i < entitiesToSimulateCount; ++i)
		{
			Entity* entity = entitiesToSimulate->getElement(i);

			if (entity->isSleeping())
			{
				continue;
			}

			if (entity->movementProperties.inverseMass != 0.0f)
			{
				entity->movementProperties.previousVelocity = entity->movementProperties.velocity;
				entity->movementProperties.previousAngularVelocity = entity->movementProperties.angularVelocity;

				entity->movementProperties.velocity =
					(entity->transform.getPosition() - entity->movementProperties.previousPosition) * inverseSubstepDeltaTime;

				mathem::Quaternion deltaOrientation = entity->transform.getOrientation().multiplyCopy(entity->movementProperties.previousOrientation.getReciprocal());
				entity->movementProperties.angularVelocity = (deltaOrientation.binary.vector * 2.0f) * inverseSubstepDeltaTime;
				if (deltaOrientation.binary.scalar < 0.0f)
				{
					entity->movementProperties.angularVelocity = -entity->movementProperties.angularVelocity;
				}
			}
		}
	}

	void solveVelocities(safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions, float substepDeltaTime)
	{
		size_t collisionsCount = collisions->getElementsCount();
		for (size_t i = 0; i < collisionsCount; ++i)
		{
			mathem::CollisionInfo<Entity*>* collisionInfo = collisions->getElementPointer(i);

			if (tryToSkipBecauseOfSleeping(collisionInfo))
			{
				continue;
			}

			mathem::GeometryTransform* objectTransform1 = collisionInfo->object1->getResultTransform();
			mathem::GeometryTransform* objectTransform2 = collisionInfo->object2->getResultTransform();
			MovementProperties* movementProperties1 = &collisionInfo->object1->movementProperties;
			MovementProperties* movementProperties2 = &collisionInfo->object2->movementProperties;
			PhysicalMaterial* physicalMaterial1 = &collisionInfo->object1->physicalMaterial;
			PhysicalMaterial* physicalMaterial2 = &collisionInfo->object2->physicalMaterial;
			float restitution = (physicalMaterial1->getRestitution() + physicalMaterial2->getRestitution()) * 0.5f;
			float dynamicFriction = (physicalMaterial1->getDynamicFriction() + physicalMaterial2->getDynamicFriction()) * 0.5f;
			float inverseMass1 = movementProperties1->inverseMass;
			float inverseMass2 = movementProperties2->inverseMass;
			
			for (size_t i = 0; i < collisionInfo->properties.contactPointsCount; ++i)
			{
				mathem::Matrix3x3 inverseI1 = collisionInfo->object1->getInverseMomentOfInertia();
				mathem::Matrix3x3 inverseI2 = collisionInfo->object2->getInverseMomentOfInertia();

				mathem::Vector3D r1 = collisionInfo->properties.contacts[i].contactPoints1;
				r1 = objectTransform1->rotation.rotate(r1);
				mathem::Vector3D r2 = collisionInfo->properties.contacts[i].contactPoints2;
				r2 = objectTransform2->rotation.rotate(r2);
				mathem::Vector3D contactPoints1GlobalCoords = r1 + objectTransform1->translation;
				mathem::Vector3D contactPoints2GlobalCoords = r2 + objectTransform2->translation;

				mathem::Vector3D deltaX = contactPoints1GlobalCoords - contactPoints2GlobalCoords;
				mathem::Vector3D n = collisionInfo->properties.normal;
				float penetrationDepth = deltaX * n;

				mathem::Vector3D r1_x_n = mathem::crossProduct(r1, n);
				float generalizedInverseMass1 = movementProperties1->inverseMass + r1_x_n * multiply(inverseI1, r1_x_n);
				mathem::Vector3D r2_x_n = mathem::crossProduct(r2, n);
				float generalizedInverseMass2 = movementProperties2->inverseMass + r2_x_n * multiply(inverseI2, r2_x_n);
				float generalizedMass = 1.0f / (generalizedInverseMass1 + generalizedInverseMass2);

				mathem::Vector3D contactPointVelocity1 =
					movementProperties1->velocity + mathem::crossProduct(movementProperties1->angularVelocity, r1);
				mathem::Vector3D contactPointVelocity2 =
					movementProperties2->velocity + mathem::crossProduct(movementProperties2->angularVelocity, r2);
				mathem::Vector3D relativeVelocity = contactPointVelocity1 - contactPointVelocity2;
				float relativeVelocityNormal = n * relativeVelocity;
				
				mathem::Vector3D previousContactPointVelocity1 =
					movementProperties1->previousVelocity + mathem::crossProduct(movementProperties1->previousAngularVelocity, r1);
				mathem::Vector3D previousContactVelocity2 =
					movementProperties2->previousVelocity + mathem::crossProduct(movementProperties2->previousAngularVelocity, r2);
				mathem::Vector3D previousRelativeVelocity = previousContactPointVelocity1 - previousContactVelocity2;
				float previousRelativeVelocityNormal = n * previousRelativeVelocity;

				mathem::Vector3D deltaVelocity = { 0.0f, 0.0f, 0.0f };

				// Dynamic friction
				mathem::Vector3D relativeVelocityTangent = relativeVelocity - n * relativeVelocityNormal;
				mathem::Vector3D tangent = relativeVelocityTangent;
				tangent.normalize();
				deltaVelocity -= tangent * std::min(
					dynamicFriction * std::fabs(collisionInfo->properties.contacts[i].normalForce / substepDeltaTime),
					relativeVelocityTangent.getLength());

				// Restitution
				deltaVelocity += n * (-relativeVelocityNormal + std::max(-restitution * previousRelativeVelocityNormal, 0.0f));

				mathem::Vector3D impulse = deltaVelocity * generalizedMass;

				if (inverseMass1 != 0.0f)
				{
					movementProperties1->velocity += impulse * inverseMass1;
					movementProperties1->angularVelocity += multiply(inverseI1, mathem::crossProduct(r1, impulse));
				}

				if (inverseMass2 != 0.0f)
				{
					movementProperties2->velocity -= impulse * inverseMass2;
					movementProperties2->angularVelocity -= multiply(inverseI2, mathem::crossProduct(r2, impulse));
				}
			}
		}
	}
}