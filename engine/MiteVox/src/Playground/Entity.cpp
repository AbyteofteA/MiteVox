#include "Entity.h"

#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Convertations.h"
#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/MiteVox/src/MiteVoxAPI.h"

namespace mitevox
{
	Entity::Entity() : collider(mathem::GeometryType::NONE)
	{
		awake(MiteVoxAPI::getSleepTime());
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
		movementProperties.inverseMass = 0.0f;
		if (mass != 0.0f)
		{
			movementProperties.inverseMass = 1.0f / mass;
		}
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
		mathem::Matrix3x3 objectOrientation = mathem::toMatrix3x3(mathem::quaternionToMatrix(transform.rotation));
		mathem::Matrix3x3 objectOrientationTransposed = getTransposed(objectOrientation);

		mathem::Matrix3x3 momentOfInertia;
		momentOfInertia = mathem::multiply(objectOrientation, movementProperties.momentOfInertia);
		momentOfInertia = mathem::multiply(momentOfInertia, objectOrientationTransposed);

		return momentOfInertia;
	}

	mathem::Matrix3x3 Entity::getInverseMomentOfInertia()
	{
		mathem::Matrix3x3 objectOrientation = mathem::toMatrix3x3(mathem::quaternionToMatrix(transform.rotation));
		mathem::Matrix3x3 objectOrientationTransposed = getTransposed(objectOrientation);

		mathem::Matrix3x3 inverseMomentOfInertia;
		inverseMomentOfInertia = mathem::multiply(objectOrientation, movementProperties.inverseMomentOfInertia);
		inverseMomentOfInertia = mathem::multiply(inverseMomentOfInertia, objectOrientationTransposed);

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

	void Entity::awake(float deltaTime)
	{
		timeUntilSleep = std::max(timeUntilSleep, deltaTime);
	}

	bool Entity::isSleeping()
	{
		if (timeUntilSleep <= 0.0f ||
			movementProperties.inverseMass == 0.0f)
		{
			return true;
		}
		return false;
	}

	void Entity::updateSleeping(float deltaTime)
	{
		mathem::Vector3D& v = movementProperties.velocity;
		mathem::Vector3D& w = movementProperties.angularVelocity;

		if (v.getLength() < MiteVoxAPI::getLinearSleepThreshold() &&
			w.getLength() < MiteVoxAPI::getAngularSleepThreshold())
		{
			timeUntilSleep -= deltaTime;
			timeUntilSleep = std::max(timeUntilSleep, 0.0f);
		}
		else
		{
			timeUntilSleep += deltaTime;
			timeUntilSleep = std::min(timeUntilSleep, MiteVoxAPI::getSleepTime());
		}
	}

	void Entity::applyDamping(float deltaTime)
	{
		mathem::Vector3D& v = movementProperties.velocity;
		mathem::Vector3D& w = movementProperties.angularVelocity;

		// Clamp to max speed
		float maxLinearSpeed = MiteVoxAPI::getMaxLinearSpeed();
		if (maxLinearSpeed > 0.0f)
		{
			v.clampLength(0.0f, maxLinearSpeed);
		}
		float maxAngularSpeed = MiteVoxAPI::getMaxAngularSpeed();
		if (maxAngularSpeed > 0.0f)
		{
			w.clampLength(0.0f, maxAngularSpeed);
		}

		// Damping
		v -= v * MiteVoxAPI::getLinearDamping() * deltaTime;
		w -= w * MiteVoxAPI::getAngularDamping() * deltaTime;
	}

	void Entity::applyForce(mathem::Vector3D force)
	{
		if (force != mathem::Vector3D::zero())
		{
			awake(MiteVoxAPI::getSleepTime());
			movementProperties.externalForces += force;
		}
	}

	void Entity::applyForceAtPoint(mathem::Vector3D force, mathem::Vector3D point)
	{
		if (force != mathem::Vector3D::zero())
		{
			awake(MiteVoxAPI::getSleepTime());
			movementProperties.externalForces += force;
			movementProperties.externalTorque += mathem::crossProduct(point - transform.translation, force);
		}
	}

	void Entity::integrateForces(float deltaTime)
	{
		mathem::Vector3D& v = movementProperties.velocity;
		mathem::Vector3D& w = movementProperties.angularVelocity;

		if (timeUntilSleep <= 0.0f)
		{
			v = { 0.0f, 0.0f, 0.0f };
			w = { 0.0f, 0.0f, 0.0f };
			return;
		}

		v += movementProperties.externalForces * (movementProperties.inverseMass * deltaTime);

		mathem::Matrix3x3 I = getMomentOfInertia();
		mathem::Matrix3x3 inverseI = getInverseMomentOfInertia();
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
				float x = box->halfSize.x() * 2.0f;
				float y = box->halfSize.y() * 2.0f;
				float z = box->halfSize.z() * 2.0f;
				movementProperties.momentOfInertia.m00() = (y * y + z * z) * getMass() / 12.0f;
				movementProperties.momentOfInertia.m11() = (x * x + z * z) * getMass() / 12.0f;
				movementProperties.momentOfInertia.m22() = (x * x + y * y) * getMass() / 12.0f;
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