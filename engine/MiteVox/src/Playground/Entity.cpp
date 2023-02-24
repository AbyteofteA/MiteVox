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
		return &renderableNode.transform;
	}

	mathem::GeometryTransform* Entity::getTransformForMove()
	{
		isMoved = false;
		return &renderableNode.transform;
	}

	bool Entity::getIsMoved()
	{
		return isMoved;
	}

	void Entity::resetTransform()
	{
		isMoved = true;
		renderableNode.transform.reset();
	}

	void Entity::setTransform(mathem::GeometryTransform transform)
	{
		isMoved = true;
		renderableNode.transform = transform;
	}

	void Entity::setTranslation(mathem::Vector3D translation)
	{
		isMoved = true;
		renderableNode.transform.translation = translation;
	}

	void Entity::setRotation(mathem::Quaternion quaternion)
	{
		isMoved = true;
		renderableNode.transform.rotation = quaternion;
	}

	void Entity::setRotation(mathem::Vector3D eulersRadians)
	{
		isMoved = true;
		renderableNode.transform.rotation.fromEulersRadians(eulersRadians.x(), eulersRadians.y(), eulersRadians.z());
	}

	void Entity::setScale(mathem::Vector3D scale)
	{
		isMoved = true;
		renderableNode.transform.scale = scale;
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

		mathem::GeometryTransform inverseTransform = renderableNode.transform.getInverse(); // 
		mathem::Vector3D min, max;
		min.setMax();
		max.setMin();
		renderableNode.getMinMaxRecursively(&inverseTransform, &min, &max);

		mathem::BoxGeometry* boundingBox = new mathem::BoxGeometry();
		boundingBox->halfSize = (max - min) * 0.5f;
		boundingBox->transform.translation = (max + min) * 0.5f;

		collider.primitives.appendElement(boundingBox);
	}
}