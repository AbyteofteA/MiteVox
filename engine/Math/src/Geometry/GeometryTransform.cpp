#include "GeometryTransform.h"

namespace mathem
{
	GeometryTransform::GeometryTransform()
	{

	}

	GeometryTransform::GeometryTransform(safety::SafeFloatArray* columnMajorArray4x4)
	{
		// TODO:
	}

	void GeometryTransform::reset()
	{
		translation = { 0.0f, 0.0f, 0.0f };
		rotation = Quaternion();
		scale = { 1.0f, 1.0f, 1.0f };
	}

	void GeometryTransform::fromArray4x4(safety::SafeFloatArray* columnMajorMatrix4x4)
	{
		// TODO:
	}

	void GeometryTransform::fromScaleArray(safety::SafeFloatArray* scaleArray)
	{
		scale.x() = scaleArray->getElement(0);
		scale.y() = scaleArray->getElement(1);
		scale.z() = scaleArray->getElement(2);
	}

	void GeometryTransform::fromRotationArray(safety::SafeFloatArray* rotationArray)
	{
		rotation.binary.scalar = rotationArray->getElement(3);
		rotation.binary.vector.x() = rotationArray->getElement(0);
		rotation.binary.vector.y() = rotationArray->getElement(1);
		rotation.binary.vector.z() = rotationArray->getElement(2);
	}

	void GeometryTransform::fromTranslationArray(safety::SafeFloatArray* translationArray)
	{
		translation.x() = translationArray->getElement(0);
		translation.y() = translationArray->getElement(1);
		translation.z() = translationArray->getElement(2);
	}

	void GeometryTransform::applyTo(Vector3D& vector)
	{
		vector.x() *= scale.x();
		vector.y() *= scale.y();
		vector.z() *= scale.z();

		Quaternion vectorAsQuaternion;
		rotation.rotate(vectorAsQuaternion);
		// TODO: vectorAsQuaternion to vector

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
		scale.x() *= otherTransform.scale.x();
		scale.y() *= otherTransform.scale.y();
		scale.z() *= otherTransform.scale.z();

		rotation = rotation.rotate(otherTransform.rotation);

		translation.x() += otherTransform.translation.x();
		translation.y() += otherTransform.translation.y();
		translation.z() += otherTransform.translation.z();
	}

	GeometryTransform GeometryTransform::operator*(GeometryTransform& otherTransform)
	{
		GeometryTransform resultTransform = *this;
		resultTransform.scale.x() *= otherTransform.scale.x();
		resultTransform.scale.y() *= otherTransform.scale.y();
		resultTransform.scale.z() *= otherTransform.scale.z();

		resultTransform.rotation = resultTransform.rotation.rotate(otherTransform.rotation);

		resultTransform.translation.x() += otherTransform.translation.x();
		resultTransform.translation.y() += otherTransform.translation.y();
		resultTransform.translation.z() += otherTransform.translation.z();

		return resultTransform;
	}
}