
#ifndef MATH_VECTOR3D_H
#define MATH_VECTOR3D_H

namespace mathem
{
	class Vector3D
	{
	public:

		float i = 0.0f;
		float j = 0.0f;
		float k = 0.0f;

		Vector3D& operator-();
		Vector3D& operator*(float multiplier);
		Vector3D& operator/(float divider);

		float getLength();
		void normalize();
	};

	Vector3D operator+(Vector3D& vectorA, Vector3D& vectorB);
	Vector3D operator-(Vector3D& vectorA, Vector3D& vectorB);

	float dotProduct(Vector3D vectorA, Vector3D vectorB);
	Vector3D crossProduct(Vector3D vectorA, Vector3D vectorB);
}

#endif