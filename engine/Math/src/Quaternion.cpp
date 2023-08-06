#include "Quaternion.h"

#include "engine/Math/src/MathConstants.h"
#include "engine/Math/src/Convertations.h"

namespace mathem
{
	Quaternion::Quaternion()
	{

	}

	Quaternion::Quaternion(Vector3D vector)
	{
		binary.scalar = 0.0f;
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

	bool Quaternion::isIdentity()
	{
		mathem::Vector3D zero = { 0.0f, 0.0f, 0.0f };
		
		if (binary.vector != zero)
		{
			return false;
		}
		if (binary.scalar != 1.0f)
		{
			return false;
		}
		return true;
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

	void Quaternion::rotateByEulers(float x, float y, float z)
	{
		Quaternion rotation;
		rotation.fromEulers(x, y, z);
		this->multiply(rotation);
	}

	void Quaternion::rotateByEulersRadians(float x, float y, float z)
	{
		Quaternion rotation;
		rotation.fromEulersRadians(x, y, z);
		this->multiply(rotation);
	}

	void Quaternion::fromEulers(float x, float y, float z)
	{
		x = toRadians(x);
		y = toRadians(y);
		z = toRadians(z);

		fromEulersRadians(x, y, z);
	}

	void Quaternion::fromEulersRadians(float x, float y, float z)
	{
		x *= 0.5f;
		y *= 0.5f;
		z *= 0.5f;
		float cosXcosY = cos(x) * cos(y);
		float cosXsinY = cos(x) * sin(y);
		float sinXsinY = sin(x) * sin(y);
		float sinXcosY = sin(x) * cos(y);
		components.s() = cosXcosY * cos(z) + sinXsinY * sin(z);
		components.x() = sinXcosY * cos(z) - cosXsinY * sin(z);
		components.y() = cosXsinY * cos(z) + sinXcosY * sin(z);
		components.z() = cosXcosY * sin(z) - sinXsinY * cos(z);
	}

	void Quaternion::toEulers(float* x, float* y, float* z)
	{
		toEulersRadians(x, y, z);

		*x = toDegrees(*x);
		*y = toDegrees(*y);
		*z = toDegrees(*z);
	}

	void Quaternion::toEulersRadians(float* x, float* y, float* z)
	{
		float q0q1 = components.s() * components.x();
		float q2q3 = components.y() * components.z();
		float q2Squared = pow(components.y(), 2.0f);

		*x = atan2(
			2.0f * (q0q1 + q2q3),
			1.0f - 2.0f * (pow(components.x(), 2.0f) + q2Squared));
		
		*y = 2.0f * std::atan2(
			std::sqrt(1.0f + 2.0f * (components.s() * components.y() - components.x() * components.z())),
			std::sqrt(1.0f - 2.0f * (components.s() * components.y() - components.x() * components.z()))) - (float)PI / 2.0f;

		*z = atan2(
			2.0f * (components.s() * components.z() + components.x() * components.y()),
			1.0f - 2.0f * (q2Squared + pow(components.z(), 2.0f)));
	}

	Vector3D Quaternion::toEulersRadians()
	{
		Vector3D resultEulersRadians;
		toEulersRadians(&resultEulersRadians.x(), &resultEulersRadians.y(), &resultEulersRadians.z());
		return resultEulersRadians;
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
		Quaternion resultQuaternion;
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

	Vector3D Quaternion::rotate(Vector3D& vector)
	{
		Quaternion vectorAsQuaternion(vector);
		vectorAsQuaternion = rotate(vectorAsQuaternion);
		return vectorAsQuaternion.binary.vector;
	}

	Quaternion Quaternion::lookRotation(Vector3D& upDirection)
	{
		mathem::Vector3D forward = { 0.0f, 0.0f, 1.0f };
		forward = rotate(forward);
		forward.normalize();
		mathem::Vector3D right = mathem::crossProduct(upDirection, forward);
		right.normalize();
		mathem::Vector3D actualUpDirection = mathem::crossProduct(forward, right);
		actualUpDirection.normalize();

		mathem::Matrix4x4 rotation;
		rotation.makeIdentity();
		rotation.m00() = right.x();
		rotation.m01() = right.y();
		rotation.m02() = right.z();
		rotation.m10() = actualUpDirection.x();
		rotation.m11() = actualUpDirection.y();
		rotation.m12() = actualUpDirection.z();
		rotation.m20() = forward.x();
		rotation.m21() = forward.y();
		rotation.m22() = forward.z();

		return matrixToQuaternion(rotation);
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