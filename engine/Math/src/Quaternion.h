
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
			Vector4D components = { 1.0f, 0.0f, 0.0f, 0.0f };
			struct Binary
			{
				float scalar;
				Vector3D vector;
			} binary;
		};

		explicit Quaternion();
		explicit Quaternion(Vector3D vector);
		explicit Quaternion(float s, float x, float y, float z);
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
