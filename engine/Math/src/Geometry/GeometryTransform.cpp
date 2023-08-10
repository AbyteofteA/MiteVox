#include "GeometryTransform.h"

#include "engine/Math/src/Convertations.h"

namespace mathem
{
	void GeometryTransform::reset()
	{
		translation.setAll(0.0f);
		rotation.reset();
		scale.setAll(1.0f);
	}

	/// <summary>
	/// May cause BUGS!!!
	/// </summary>
	/// <returns></returns>
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
		vector = rotation.rotate(vector);

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

	void GeometryTransform::applyInverseTo(Vector3D& vector)
	{
		vector.x() -= translation.x();
		vector.y() -= translation.y();
		vector.z() -= translation.z();

		// TODO: check rotation
		vector = rotation.getConjugate().rotate(vector);

		vector.x() /= scale.x();
		vector.y() /= scale.y();
		vector.z() /= scale.z();
	}

	Vector3D GeometryTransform::applyInverseToCopy(Vector3D vector)
	{
		Vector3D resultVector = vector;
		this->applyInverseTo(resultVector);
		return resultVector;
	}

	void GeometryTransform::applyTo(TriangleGeometry3D& triangle)
	{
		this->applyTo(triangle.point1);
		this->applyTo(triangle.point2);
		this->applyTo(triangle.point3);
	}

	TriangleGeometry3D GeometryTransform::applyToCopy(TriangleGeometry3D triangle)
	{
		TriangleGeometry3D resultTriangle = triangle;
		this->applyTo(resultTriangle);
		return resultTriangle;
	}

	void GeometryTransform::operator*=(GeometryTransform& otherTransform)
	{
		*this = *this * otherTransform;
	}

	GeometryTransform GeometryTransform::operator*(GeometryTransform& otherTransform)
	{
		Matrix4x4 thisMatrix = mathem::transformToMatrix(*this);
		Matrix4x4 otherMatrix = mathem::transformToMatrix(otherTransform);
		Matrix4x4 resultMatrix = mathem::multiply(otherMatrix, thisMatrix);
		return mathem::matrixToTransform(resultMatrix);
	}
}