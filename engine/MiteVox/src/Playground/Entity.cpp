#include "Entity.h"

#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Convertations.h"
#include "engine/MiteVox/src/Physics/MovementProperties.h"

namespace mitevox
{
	Entity::Entity() : collider(mathem::GeometryType::NONE)
	{

	}

	mathem::GeometryTransform* Entity::getTransform()
	{
		return &transform;
	}

	bool Entity::hasCamera()
	{
		return renderableNode->hasCameraRecursively();
	}

	mathem::GeometryTransform Entity::getCamera(render::Camera** cameraResult)
	{
		return renderableNode->getCameraRecursively(cameraResult, &transform);
	}

	mathem::Vector3D Entity::getViewRay()
	{
		mathem::Vector3D viewRay = { 0.0f, 0.0f, -1.0f };
		render::Camera* c = nullptr;
		mathem::GeometryTransform cameraTransform = getCamera(&c);
		viewRay = cameraTransform.rotation.rotate(viewRay);
		return viewRay;
	}

	void Entity::resetTransform()
	{
		transform.reset();
	}

	void Entity::setTransform(mathem::GeometryTransform transform)
	{
		transform = transform;
	}

	void Entity::setTranslation(mathem::Vector3D translation)
	{
		transform.translation = translation;
	}

	void Entity::setRotation(mathem::Quaternion quaternion)
	{
		transform.rotation = quaternion;
	}

	void Entity::setRotation(mathem::Vector3D eulersRadians)
	{
		transform.rotation.fromEulersRadians(eulersRadians.x(), eulersRadians.y(), eulersRadians.z());
	}

	void Entity::setScale(mathem::Vector3D scale)
	{
		transform.scale = scale;
	}

	void Entity::setMass(float mass)
	{
		if (mass == 0.0f)
		{
			movementProperties.inverseMass = 0.0f;
			return;
		}
		movementProperties.inverseMass = 1.0f / mass;
		computeMomentOfInertia();
	}

	float Entity::getMass()
	{
		if (movementProperties.inverseMass == 0.0f)
		{
			return 0.0f;
		}
		return 1.0f / movementProperties.inverseMass;
	}

	void Entity::computeMomentOfInertia()
	{
		movementProperties.momentOfInertia.makeZero();
		movementProperties.inverseMomentOfInertia.makeZero();
		float mass = getMass();

		if (mass == 0.0f)
		{
			return;
		}

		switch (collider.getType())
		{
		case mathem::GeometryType::PRIMITIVES:
			computeMomentOfInertiaForPrimitives();
			break;

		case mathem::GeometryType::MESH:
			computeMomentOfInertiaForMesh();
			break;

		case mathem::GeometryType::SKELETON:
			computeMomentOfInertiaForSkeleton();
			break;

		case mathem::GeometryType::NONE:
		case mathem::GeometryType::POINT:
		default:
			break;
		}
	}

	mathem::Matrix3x3 Entity::getMomentOfInertia()
	{
		mathem::Matrix3x3 objectOrientation = mathem::toMatrix3x3(mathem::quaternionToMatrix(renderableNode->transform.rotation));
		mathem::Matrix3x3 objectOrientationTransposed = objectOrientation;
		objectOrientationTransposed.transpose();

		mathem::Matrix3x3 momentOfInertia;
		momentOfInertia = mathem::multiply(objectOrientation, movementProperties.momentOfInertia);
		momentOfInertia = mathem::multiply(movementProperties.momentOfInertia, objectOrientationTransposed);

		return momentOfInertia;
	}

	mathem::Matrix3x3 Entity::getInverseMomentOfInertia()
	{
		mathem::Matrix3x3 objectOrientation = mathem::toMatrix3x3(mathem::quaternionToMatrix(renderableNode->transform.rotation));
		mathem::Matrix3x3 objectOrientationTransposed = objectOrientation;
		objectOrientationTransposed.transpose();

		mathem::Matrix3x3 inverseMomentOfInertia;
		inverseMomentOfInertia = mathem::multiply(objectOrientation, movementProperties.inverseMomentOfInertia);
		inverseMomentOfInertia = mathem::multiply(movementProperties.inverseMomentOfInertia, objectOrientationTransposed);

		return inverseMomentOfInertia;
	}

	mathem::ComplexGeometry* Entity::getCollider()
	{
		return &collider;
	}

	void Entity::tryGenerateHitbox()
	{
		if (collider.getPrimitivesCount() != 0)
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

		collider.setType(mathem::GeometryType::PRIMITIVES);
		collider.appendPrimitive(boundingBox);

		computeMomentOfInertia();
	}

	void Entity::applyForce(mathem::Vector3D force)
	{
		movementProperties.externalForces += force;
	}

	void Entity::applyForceAtPoint(mathem::Vector3D force, mathem::Vector3D point)
	{
		movementProperties.externalForces += force;
		movementProperties.externalTorque += mathem::crossProduct(point - transform.translation, force);
	}

	void Entity::integrateForces(float deltaTime)
	{
		movementProperties.velocity += movementProperties.externalForces / getMass() * deltaTime;

		mathem::Matrix3x3 I = getMomentOfInertia();
		mathem::Matrix3x3 inverseI = getInverseMomentOfInertia();
		mathem::Vector3D& w = movementProperties.angularVelocity;
		mathem::Vector3D T = movementProperties.externalTorque;

		w += mathem::multiply(inverseI, (T - mathem::crossProduct(w, mathem::multiply(I, w)))) * deltaTime;
	}

	void Entity::integrateGravity(mathem::Vector3D gravity, float deltaTime)
	{
		movementProperties.velocity += gravity * deltaTime;
	}

	void Entity::integrateVelocities(float deltaTime)
	{
		transform.translation += movementProperties.velocity * deltaTime;
		
		mathem::Quaternion deltaOrientation(
			movementProperties.angularVelocity.x(),
			movementProperties.angularVelocity.y(),
			movementProperties.angularVelocity.z(),
			0.0f);
		mathem::Quaternion worldDeltaOrientation = deltaOrientation.multiplyCopy(transform.rotation);
		transform.rotation.components += worldDeltaOrientation.components * 0.5f * deltaTime;
		transform.rotation.normalize();
	}

	void Entity::resetForces()
	{
		movementProperties.externalForces = { 0.0f, 0.0f, 0.0f };
		movementProperties.externalTorque = { 0.0f, 0.0f, 0.0f };
	}

	Node* Entity::tryAttachNewRenderableNode()
	{
		if (renderableNode == nullptr)
		{
			renderableNode = new Node();
		}
		return renderableNode;
	}

	/// <summary>
	/// TODO: implement other primitives
	/// TODO: add support of off-center primitives
	/// </summary>
	void Entity::computeMomentOfInertiaForPrimitives()
	{
		size_t colliderPrimitivesCount = collider.getPrimitivesCount();
		for (size_t i = 0; i < colliderPrimitivesCount; ++i)
		{
			mathem::GeometryPrimitiveBase* primitive = collider.getPrimitive(i);

			switch (primitive->getType())
			{
			case mathem::GeometryPrimitiveType::BOX:
			{
				mathem::BoxGeometry* box = (mathem::BoxGeometry*)primitive;
				movementProperties.momentOfInertia.m00() = (box->halfSize.y() * box->halfSize.y() + box->halfSize.z() * box->halfSize.z()) * getMass() / 12.0f;
				movementProperties.momentOfInertia.m11() = (box->halfSize.x() * box->halfSize.x() + box->halfSize.z() * box->halfSize.z()) * getMass() / 12.0f;
				movementProperties.momentOfInertia.m22() = (box->halfSize.x() * box->halfSize.x() + box->halfSize.y() * box->halfSize.y()) * getMass() / 12.0f;
				movementProperties.inverseMomentOfInertia = mathem::getInverse(movementProperties.momentOfInertia);
				break;
			}

			default:
				break;
			}
		}
	}

	void Entity::computeMomentOfInertiaForMesh()
	{
		// TODO:
	}

	void Entity::computeMomentOfInertiaForSkeleton()
	{
		// TODO:
	}
}