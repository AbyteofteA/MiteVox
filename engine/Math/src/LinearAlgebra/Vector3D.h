
#ifndef MATH_VECTOR3D_H
#define MATH_VECTOR3D_H

namespace mathem
{

	struct Vector3D
	{
		float i = 0.;
		float j = 0.;
		float k = 0.;
		float length = 0.;

		float getLength()
		{
			length = (float)sqrt(pow(i, 2) + pow(i, 2) + pow(i, 2));

			return length;
		}

		void normalize()
		{
			getLength();

			i /= length;
			j /= length;
			k /= length;

			length = 1;
		}
	};

}

#endif