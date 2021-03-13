
#ifndef MATH_VECTOR3D_H
#define MATH_VECTOR3D_H

namespace mathem
{
	class Vector3D
	{
	public:

		float i = 0.;
		float j = 0.;
		float k = 0.;

		inline float getLength();
		inline void normalize();
	};


	// IMPLEMENTATION BELOW //


	inline float Vector3D::getLength()
	{
		return (float)sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2));
	}

	inline void Vector3D::normalize()
	{
		float length = getLength();
		i /= length;
		j /= length;
		k /= length;
	}
}

#endif