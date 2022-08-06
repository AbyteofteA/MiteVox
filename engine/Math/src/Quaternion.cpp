#include "Quaternion.h"

namespace mathem
{
	Quaternion::Quaternion()
	{

	}

	Quaternion::Quaternion(Vector3D vector)
	{
		binary.scalar = 0;
		binary.vector = vector;
	}

	Quaternion::Quaternion(float x, float y, float z, float s)
	{
		binary.vector.x() = x;
		binary.vector.y() = y;
		binary.vector.z() = z;
		binary.scalar = s;
	}

	Quaternion::Quaternion(safety::SafeFloatArray* quaternionArray)
	{
		if (quaternionArray->getElementsCount() >= 4)
		{
			components.x() = quaternionArray->getElement(0);
			components.y() = quaternionArray->getElement(1);
			components.z() = quaternionArray->getElement(2);
			components.s() = quaternionArray->getElement(3);
		}
	}

	void Quaternion::reset()
	{
		binary.vector.x() = 0.0f;
		binary.vector.y() = 0.0f;
		binary.vector.z() = 0.0f;
		binary.scalar = 1.0f;
	}

	float Quaternion::getLength()
	{
		return components.getLength();
	}

	float Quaternion::getLengthSquared()
	{
		return components.getLengthSquared();
	}

	void Quaternion::normalize()
	{
		components.normalize();
	}

	Quaternion Quaternion::getConjugate()
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.binary.vector = -resultQuaternion.binary.vector;
		return resultQuaternion;
	}

	Quaternion Quaternion::getReciprocal()
	{
		Quaternion resultQuaternion = getConjugate();
		return resultQuaternion / getLengthSquared();
	}

	Quaternion Quaternion::operator*(float multiplier)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.components *= multiplier;
		return resultQuaternion;
	}

	void Quaternion::operator*=(float multiplier)
	{
		this->components *= multiplier;
	}

	float Quaternion::operator*(Quaternion& otherVector)
	{
		return this->components * otherVector.components;
	}

	Quaternion Quaternion::multiplyCopy(Quaternion& otherQuaternion)
	{
		Quaternion resultQuaternion = *this;
		float scalar =
			this->binary.scalar * otherQuaternion.binary.scalar -
			this->binary.vector * otherQuaternion.binary.vector;
		Vector3D tmpVector1 = otherQuaternion.binary.vector * this->binary.scalar;
		Vector3D tmpVector2 = this->binary.vector * otherQuaternion.binary.scalar;
		Vector3D tmpVector3 = crossProduct(this->binary.vector, otherQuaternion.binary.vector);
		resultQuaternion.binary.scalar = scalar;
		resultQuaternion.binary.vector = tmpVector1;
		resultQuaternion.binary.vector += tmpVector2;
		resultQuaternion.binary.vector += tmpVector3;
		return resultQuaternion;
	}

	void Quaternion::multiply(Quaternion& otherQuaternion)
	{
		float scalar =
			this->binary.scalar * otherQuaternion.binary.scalar -
			this->binary.vector * otherQuaternion.binary.vector;
		Vector3D tmpVector1 = otherQuaternion.binary.vector * this->binary.scalar;
		Vector3D tmpVector2 = this->binary.vector * otherQuaternion.binary.scalar;
		Vector3D tmpVector3 = crossProduct(this->binary.vector, otherQuaternion.binary.vector);
		this->binary.scalar = scalar;
		this->binary.vector = tmpVector1;
		this->binary.vector += tmpVector2;
		this->binary.vector += tmpVector3;
	}

	Quaternion Quaternion::rotate(Quaternion& otherQuaternion)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.multiply(otherQuaternion);
		Quaternion thisQuaternionReciprocal = this->getReciprocal();
		resultQuaternion.multiply(thisQuaternionReciprocal);
		return resultQuaternion;
	}

	Quaternion Quaternion::operator/(float divider)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.components /= divider;
		return resultQuaternion;
	}

	void Quaternion::operator/=(float divider)
	{
		this->components /= divider;
	}

	Quaternion Quaternion::operator+(float value)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.components += value;
		return resultQuaternion;
	}

	void Quaternion::operator+=(float value)
	{
		this->components += value;
	}

	Quaternion Quaternion::operator+(Quaternion& otherQuaternion)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.components += otherQuaternion.components;
		return resultQuaternion;
	}

	void Quaternion::operator+=(Quaternion& otherQuaternion)
	{
		this->components += otherQuaternion.components;
	}

	Quaternion Quaternion::operator-(float value)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.components -= value;
		return resultQuaternion;
	}

	void Quaternion::operator-=(float value)
	{
		this->components -= value;
	}

	Quaternion Quaternion::operator-(Quaternion& otherQuaternion)
	{
		Quaternion resultQuaternion = *this;
		resultQuaternion.components -= otherQuaternion.components;
		return resultQuaternion;
	}

	void Quaternion::operator-=(Quaternion& otherQuaternion)
	{
		this->components -= otherQuaternion.components;
	}

}