#include "GeometryTransform.h"

namespace mathem
{
	void GeometryTransform::reset()
	{
		translation.setAll(0.0f);
		rotation.reset();
		scale.setAll(1.0f);
	}

	GeometryTransform GeometryTransform::getInverse()
	{
		GeometryTransform inverseTransform;
		inverseTransform.translation = -translation;
		inverseTransform.rotation = rotation.getConjugate();
		inverseTransform.scale.x() = 1.0f / scale.x();
		inverseTransform.scale.y() = 1.0f / scale.y();
		inverseTransform.scale.z() = 1.0f / scale.z();
		return inverseTransform;
	}

	void GeometryTransform::applyTo(Vector3D& vector)
	{
		vector.x() *= scale.x();
		vector.y() *= scale.y();
		vector.z() *= scale.z();

		// TODO: check rotation
		Quaternion vectorAsQuaternion(vector);
		vectorAsQuaternion = rotation.rotate(vectorAsQuaternion);
		//vectorAsQuaternion = vectorAsQuaternion.rotate(rotation);
		vector = vectorAsQuaternion.binary.vector;

		vector.x() += translation.x();
		vector.y() += translation.y();
		vector.z() += translation.z();
	}

	Vector3D GeometryTransform::applyToCopy(Vector3D vector)
	{
		Vector3D resultVector = vector;
		this->applyTo(resultVector);
		return resultVector;
	}

	void GeometryTransform::applyTo(TriangleGeometry& triangle)
	{
		this->applyTo(triangle.point1);
		this->applyTo(triangle.point2);
		this->applyTo(triangle.point3);
	}

	TriangleGeometry GeometryTransform::applyToCopy(TriangleGeometry triangle)
	{
		TriangleGeometry resultTriangle = triangle;
		this->applyTo(resultTriangle);
		return resultTriangle;
	}

	void GeometryTransform::operator*=(GeometryTransform& otherTransform)
	{
		this->scale.x() *= otherTransform.scale.x();
		this->scale.y() *= otherTransform.scale.y();
		this->scale.z() *= otherTransform.scale.z();

		this->rotation.multiply(otherTransform.rotation);

		Quaternion translationQuaternion(otherTransform.translation);
		translationQuaternion = this->rotation.rotate(translationQuaternion);
		this->translation += translationQuaternion.binary.vector;
	}

	GeometryTransform GeometryTransform::operator*(GeometryTransform& otherTransform)
	{
		GeometryTransform resultTransform;
		resultTransform.scale.x() = this->scale.x() * otherTransform.scale.x();
		resultTransform.scale.y() = this->scale.y() * otherTransform.scale.y();
		resultTransform.scale.z() = this->scale.z() * otherTransform.scale.z();

		resultTransform.rotation = this->rotation.multiplyCopy(otherTransform.rotation);

		Quaternion translationQuaternion(otherTransform.translation);
		translationQuaternion = this->rotation.rotate(translationQuaternion);
		resultTransform.translation = this->translation + translationQuaternion.binary.vector;
		
		// BUG: scale may influence translation like rotation does

		return resultTransform;
	}
}