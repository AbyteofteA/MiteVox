#include "Entity.h"

#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Convertations.h"
#include "engine/Physics/src/MovementProperties.h"

namespace mitevox
{
	mathem::GeometryTransform* Entity::getTransform()
	{
		return &transform;
	}

	mathem::GeometryTransform* Entity::getTransformForMove()
	{
		isMoved = false;
		return &transform;
	}

	bool Entity::getIsMoved()
	{
		return isMoved;
	}

	void Entity::resetTransform()
	{
		isMoved = true;
		transform.reset();
	}

	void Entity::setTransform(mathem::GeometryTransform transform)
	{
		isMoved = true;
		transform = transform;
	}

	void Entity::setTranslation(mathem::Vector3D translation)
	{
		isMoved = true;
		transform.translation = translation;
	}

	void Entity::setRotation(mathem::Quaternion quaternion)
	{
		isMoved = true;
		transform.rotation = quaternion;
	}

	void Entity::setRotation(mathem::Vector3D eulersRadians)
	{
		isMoved = true;
		transform.rotation.fromEulersRadians(eulersRadians.x(), eulersRadians.y(), eulersRadians.z());
	}

	void Entity::setScale(mathem::Vector3D scale)
	{
		isMoved = true;
		transform.scale = scale;
	}

	mathem::Matrix3x3 Entity::getInverseMomentOfInertia()
	{
		// TODO:
		mathem::Matrix3x3 momentOfInertia;

		if (movementProperties.inverseMomentOfInertia == 0.0f)
		{
			momentOfInertia.makeZero();
			return momentOfInertia;
		}

		momentOfInertia.m00() = (20.0f / 12.0f) * movementProperties.getMass();
		momentOfInertia.m01() = 0.0f;
		momentOfInertia.m02() = 0.0f;
		momentOfInertia.m10() = 0.0f;
		momentOfInertia.m11() = (20.0f / 12.0f) * movementProperties.getMass();
		momentOfInertia.m12() = 0.0f;
		momentOfInertia.m20() = 0.0f;
		momentOfInertia.m21() = 0.0f;
		momentOfInertia.m22() = (20.0f / 12.0f) * movementProperties.getMass();

		//mathem::Matrix3x3 objectOrientation = mathem::toMatrix3x3(mathem::quaternionToMatrix(renderableNode.transform.rotation));
		//mathem::Matrix3x3 objectOrientationInverse = mathem::getInverse(objectOrientation);
		//momentOfInertia = mathem::multiply(momentOfInertia, objectOrientationInverse);
		//momentOfInertia = mathem::multiply(objectOrientation, momentOfInertia);

		return getInverse(momentOfInertia);
	}

	mathem::ComplexGeometry* Entity::getCollider()
	{
		return &collider;
	}

	void Entity::tryGenerateHitbox()
	{
		if (collider.primitives.getElementsCount() != 0)
		{
			return;
		}

		mathem::GeometryTransform zeroTransform; // 
		mathem::Vector3D min, max;
		min.setMax();
		max.setMin();
		renderableNode->getMinMaxRecursively(&zeroTransform, &min, &max);

		mathem::BoxGeometry* boundingBox = new mathem::BoxGeometry();
		boundingBox->halfSize = (max - min) * 0.5f;
		boundingBox->transform.translation = (max + min) * 0.5f;

		collider.primitives.appendElement(boundingBox);
	}
}