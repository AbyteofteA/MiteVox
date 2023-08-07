#include "separateObjects.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Quaternion.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/Physics/MovementProperties.h"

namespace mitevox
{
	void separateObjects(mathem::CollisionInfo<Entity*>* collisionInfo)
	{
		mathem::GeometryTransform* objectTransform1 = collisionInfo->object1->getTransform();
		mathem::GeometryTransform* objectTransform2 = collisionInfo->object2->getTransform();
		MovementProperties* movementProperties1 = &collisionInfo->object1->movementProperties;
		MovementProperties* movementProperties2 = &collisionInfo->object2->movementProperties;

		mathem::Vector3D separationVector = -collisionInfo->properties.normal * collisionInfo->properties.penetrationDepth;
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
}
