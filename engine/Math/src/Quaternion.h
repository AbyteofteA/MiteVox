
#ifndef QUATERNION_H
#define QUATERNION_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Matrix.h"

namespace mathem
{
	class Quaternion
	{
	public:

		union
		{
			Vector4D components = { 0.0f, 0.0f, 0.0f, 1.0f };
			struct Binary
			{
				Vector3D vector;
				float scalar;
			} binary;
		};

		explicit Quaternion();
		explicit Quaternion(Vector3D vector);
		explicit Quaternion(float x, float y, float z, float s);
		explicit Quaternion(safety::SafeFloatArray* quaternionArray);
		void reset();
		float getLength();
		float getLengthSquared();
		void normalize();
		Quaternion getConjugate();
		Quaternion getReciprocal();

		Quaternion operator*(float multiplier);
		void operator*=(float multiplier);
		float operator*(Quaternion& otherQuaternion);
		Quaternion multiplyCopy(Quaternion& otherQuaternion);
		void multiply(Quaternion& otherQuaternion);
		Quaternion rotate(Quaternion& otherQuaternion);

		Quaternion operator/(float divider);
		void operator/=(float divider);

		Quaternion operator+(float value);
		void operator+=(float value);
		Quaternion operator+(Quaternion& otherQuaternion);
		void operator+=(Quaternion& otherQuaternion);

		Quaternion operator-(float value);
		void operator-=(float value);
		Quaternion operator-(Quaternion& otherQuaternion);
		void operator-=(Quaternion& otherQuaternion);

	private:
	};
}

#endif
